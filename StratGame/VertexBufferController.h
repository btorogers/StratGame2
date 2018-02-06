#pragma once

#include "Includes.h"
#include "Vertex.h"

struct RenderElement {
	unsigned int startIndex, length;
	D3D11_PRIMITIVE_TOPOLOGY primitiveType;
};

class VertexBufferController {
public:
	VertexBufferController(ID3D11Device *dev, ID3D11DeviceContext *devcon);
	~VertexBufferController();
	unsigned int AppendVertices(Vertex * vertices, unsigned int length, D3D11_PRIMITIVE_TOPOLOGY primitiveType);
	void DeleteElement(unsigned int index);
	void Render();
private:
	ID3D11Device *dev;
	ID3D11DeviceContext *devcon;
	std::vector<RenderElement> elements;
	ID3D11Buffer *buffer;
	unsigned int maxVert;
	unsigned int maxIndex;
};