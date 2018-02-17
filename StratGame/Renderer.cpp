#include "Renderer.h"
#include "GameController.h"

Renderer::Renderer(HWND hWnd, GameController* game): game(game) {
	// swap chain
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = SCREEN_WIDTH;
	scd.BufferDesc.Height = SCREEN_HEIGHT;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = MULTISAMPLE_COUNT;
	scd.Windowed = FALSE;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG,
		NULL, NULL, D3D11_SDK_VERSION, &scd, &swapchain, &dev, NULL, &devcon);

	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backBuffer);
	pBackBuffer->Release();

	// depth buffer
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

	dev->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);

	// depth stencil
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	dev->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	devcon->OMSetDepthStencilState(depthStencilState, 1);

	// depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = (MULTISAMPLE_COUNT > 1) ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	dev->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	devcon->OMSetRenderTargets(1, &backBuffer, depthStencilView);

	// rasterizer
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

	// viewport
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

	D3DXMatrixPerspectiveFovLH(&projection, (float)D3DX_PI/4.0f, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 1000.0f);
	D3DXMatrixIdentity(&position);
	D3DXMatrixIdentity(&rotation);

	bgcolor = D3DXCOLOR(0.35f, 0.65f, 1.0f, 1.0f);
	lightColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	lightDirection = D3DXVECTOR3(0.25f, -0.75f, 0.5f);
	D3DXVec3Normalize(&lightDirection, &lightDirection);
	InitShaders();
	InitBuffers();
}

Renderer::~Renderer() {
	delete camera;
	delete vbc;
	swapchain->SetFullscreenState(FALSE, NULL);
	pLayout->Release();
	pVS->Release();
	pPS->Release();
	matrixBuffer->Release();
	lightBuffer->Release();
	swapchain->Release();
	rasterState->Release();
	depthStencilView->Release();
	depthStencilState->Release();
	depthStencilBuffer->Release();
	backBuffer->Release();
	dev->Release();
	devcon->Release();
}

void Renderer::UpdateMatrices() {
	D3D11_MAPPED_SUBRESOURCE mapSub;
	// View matrices
	static MatrixBufferStruct matricesOld;
	MatrixBufferStruct matrices;

	D3DXMATRIX view = camera->GetViewMatrix();
	D3DXMatrixTranspose(&matrices.rotation, &rotation);
	D3DXMatrixTranspose(&matrices.position, &position);
	D3DXMatrixTranspose(&matrices.view, &view);
	D3DXMatrixTranspose(&matrices.projection, &projection);

	if (matrices.rotation != matricesOld.rotation || matrices.position != matricesOld.position 
		|| matrices.view != matricesOld.view || matrices.projection != matricesOld.projection) {
		devcon->Map(matrixBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSub);
		memcpy(mapSub.pData, &matrices, sizeof(MatrixBufferStruct));
		devcon->Unmap(matrixBuffer, NULL);
		devcon->VSSetConstantBuffers(0, 1, &matrixBuffer);
		matricesOld = matrices;
	}

	// Light information
	ZeroMemory(&mapSub, sizeof(D3D11_MAPPED_SUBRESOURCE));
	static LightBufferStruct lightingOld;
	LightBufferStruct lighting = { lightColor, lightDirection, 0.0f };
	if (lighting.diffuseColor != lightingOld.diffuseColor || lighting.lightDirection != lightingOld.lightDirection) {
		devcon->Map(lightBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapSub);
		memcpy(mapSub.pData, &lighting, sizeof(LightBufferStruct));
		devcon->Unmap(lightBuffer, NULL);
		devcon->PSSetConstantBuffers(0, 1, &lightBuffer);
		lightingOld = lighting;
	}
}

void Renderer::InitShaders() {
	ID3D10Blob *VS, *PS;
	D3DX11CompileFromFile("shaders.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile("shaders.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } };

	dev->CreateInputLayout(ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
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

	D3D11_BUFFER_DESC lightbd;
	ZeroMemory(&lightbd, sizeof(lightbd));

	lightbd.Usage = D3D11_USAGE_DYNAMIC;
	lightbd.ByteWidth = sizeof(LightBufferStruct);
	lightbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&lightbd, NULL, &lightBuffer);
}

void Renderer::RenderFrame() {
	devcon->ClearRenderTargetView(backBuffer, bgcolor);
	devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	camera->Render();

	D3DXMatrixIdentity(&rotation);
	D3DXMatrixIdentity(&position);
	UpdateMatrices();
	vbc->RenderStatic();

	game->RenderObjects();

	swapchain->Present(1, 0);
}

VertexBufferController* Renderer::GetVertexBufferController()
{
	return vbc;
}

Camera* Renderer::GetCamera() {
	return camera;
}

void Renderer::SetRotationMatrix(D3DXMATRIX rotation) {
	this->rotation = rotation;
	UpdateMatrices();
}

void Renderer::SetPositionMatrix(D3DXMATRIX position) {
	this->position = position;
	UpdateMatrices();
}

D3DXMATRIX Renderer::GetProjectionMatrix() {
	return projection;
}