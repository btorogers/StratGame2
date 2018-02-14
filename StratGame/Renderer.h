#pragma once

#include "Includes.h"
#include "Camera.h"
#include "VertexBufferController.h"

class GameController;

class Renderer {
public:
	Renderer(HWND hWnd, GameController* game);
	~Renderer();
	void UpdateMatrices();
	void RenderFrame();

	VertexBufferController* GetVertexBufferController();
	void SetRotationMatrix(D3DXMATRIX rotation);
	void SetPositionMatrix(D3DXMATRIX position);

	static const int SCREEN_WIDTH = 2560;
	static const int SCREEN_HEIGHT = 1440;
	static const int MULTISAMPLE_COUNT = 4;
	static const D3D11_FILL_MODE FILL_MODE = D3D11_FILL_SOLID;
private:
	void InitShaders();
	void InitBuffers();

	struct MatrixBufferStruct { D3DXMATRIX rotation;  D3DXMATRIX position; D3DXMATRIX view; D3DXMATRIX projection; };
	struct LightBufferStruct { D3DXVECTOR4 diffuseColor; D3DXVECTOR3 lightDirection; float padding; };

	IDXGISwapChain* swapchain;
	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;
	ID3D11RenderTargetView* backBuffer;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RasterizerState* rasterState;
	ID3D11VertexShader* pVS;
	ID3D11PixelShader* pPS;
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* lightBuffer;
	ID3D11InputLayout* pLayout;

	Camera* camera;
	D3DXMATRIX projection, position, rotation;
	GameController* game;

	D3DXCOLOR bgcolor;
	D3DXVECTOR4 lightColor;
	D3DXVECTOR3 lightDirection;
	VertexBufferController* vbc;
};