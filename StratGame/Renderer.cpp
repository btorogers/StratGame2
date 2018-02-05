#include "Renderer.h"

#define Color(r,g,b) D3DXCOLOR((float)r / 256.0f, (float)g / 256.0f, (float)b / 256.0f, 1.0f)
#define RANDCOL D3DXCOLOR((float)rand() / 32767.0f, (float)rand() / 32767.0f, (float)rand() / 32767.0f, 1.0f)
#define RANDINT(x) ((float)rand() / 32767.0f * (float)x)
#define RANDPOSNEG(x) (RANDINT(x*2)-x)

Renderer::Renderer(HWND hWnd) {
	HRESULT r;
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferDesc.Width = SCREEN_WIDTH;
	scd.BufferDesc.Height = SCREEN_HEIGHT;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = MULTISAMPLE_COUNT;
	scd.Windowed = FALSE;                                    // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG,
		NULL, NULL, D3D11_SDK_VERSION, &scd, &swapchain, &dev, NULL, &devcon);

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backBuffer);
	pBackBuffer->Release();

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = SCREEN_WIDTH;
	depthBufferDesc.Height = SCREEN_HEIGHT;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = MULTISAMPLE_COUNT;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	r = dev->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
	if (FAILED(r))
	{
		MessageBox(NULL, "failed", "oops", MB_OK);
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	dev->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	devcon->OMSetDepthStencilState(depthStencilState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = (MULTISAMPLE_COUNT > 1) ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	dev->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	devcon->OMSetRenderTargets(1, &backBuffer, depthStencilView);

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = MULTISAMPLE_COUNT > 1;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = FILL_MODE;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = MULTISAMPLE_COUNT > 1;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	dev->CreateRasterizerState(&rasterDesc, &rasterState);

	devcon->RSSetState(rasterState);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	devcon->RSSetViewports(1, &viewport);

	camera = new Camera;
	vbc = new VertexBufferController(dev, devcon);

	D3DXMatrixPerspectiveFovLH(&projectionMatrix, (float)D3DX_PI/4.0f, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 1000.0f);
	D3DXMatrixIdentity(&worldMatrix);

	Cube c(0.0f, 0.0f, 0.0f, 2.0f, Color(128, 0, 0));
	Cube c1(2.0f, 0.0f, 0.0f, 1.0f, Color(0, 128, 0));
	Cube c2(0.0f, 2.0f, 0.0f, 1.0f, Color(0, 0, 128));

	Vertex* v;
	unsigned int length = 0;
	v = c.GetFullCoverStrip(&length);
	vbc->AppendVertices(v, length, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	v = c1.GetFullCoverStrip(&length);
	vbc->AppendVertices(v, length, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	v = c2.GetFullCoverStrip(&length);
	vbc->AppendVertices(v, length, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	bgcolor = Color(94, 174, 255);

	InitShaders();
	InitBuffers();
}

Renderer::~Renderer() {
	swapchain->SetFullscreenState(FALSE, NULL);
	pLayout->Release();
	pVS->Release();
	pPS->Release();
	matrixBuffer->Release();
	swapchain->Release();
	rasterState->Release();
	depthStencilView->Release();
	depthStencilState->Release();
	depthStencilBuffer->Release();
	backBuffer->Release();
	dev->Release();
	devcon->Release();
	delete camera;
	delete vbc;
}

void Renderer::UpdateMatrices() {
	D3D11_MAPPED_SUBRESOURCE matrixMapSub;
	MatrixBufferStruct matrices;

	D3DXMATRIX viewMatrix;
	camera->GetViewMatrix(viewMatrix);

	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&matrices.world, &worldMatrix);
	D3DXMatrixTranspose(&matrices.view, &viewMatrix);
	D3DXMatrixTranspose(&matrices.projection, &projectionMatrix);

	devcon->Map(matrixBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &matrixMapSub);
	memcpy(matrixMapSub.pData, &matrices, sizeof(MatrixBufferStruct));
	devcon->Unmap(matrixBuffer, NULL);

	devcon->VSSetConstantBuffers(0, 1, &matrixBuffer);
}

void Renderer::InitShaders() {
	// load and compile the two shaders
	ID3D10Blob *VS, *PS;
	D3DX11CompileFromFile("shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile("shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	// encapsulate both shaders into shader objects
	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	// set the shader objects
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };

	dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	devcon->IASetInputLayout(pLayout);
}

void Renderer::InitBuffers() {
	// create the matrix buffer
	D3D11_BUFFER_DESC matrixbd;
	ZeroMemory(&matrixbd, sizeof(matrixbd));

	matrixbd.Usage = D3D11_USAGE_DYNAMIC;
	matrixbd.ByteWidth = sizeof(MatrixBufferStruct);
	matrixbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&matrixbd, NULL, &matrixBuffer);
}

void Renderer::RenderFrame() {
	devcon->ClearRenderTargetView(backBuffer, bgcolor);
	devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Generate the view matrix based on the camera's position.
	camera->Render();

	UpdateMatrices();

	vbc->Render();

	// switch the back buffer and the front buffer
	swapchain->Present(0, 0);
}

void Renderer::AddRandomCube() {
	Cube c(RANDPOSNEG(5), RANDPOSNEG(5), RANDPOSNEG(5), RANDINT(2), RANDCOL);
	Vertex* v;
	unsigned int length = 0;
	v = c.GetFullCoverStrip(&length);
	vbc->AppendVertices(v, length, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}
