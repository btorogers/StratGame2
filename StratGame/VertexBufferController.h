#pragma once

#include "Includes.h"
#include "Vertex.h"

class VertexBufferController {
public:
	enum ACTION_IF_BUSY {
		WAIT_UNTIL_FREE = 0,
		RETURN_FALSE = 1
	};

	VertexBufferController(ID3D11Device* dev, ID3D11DeviceContext* devcon);
	~VertexBufferController();
	bool lock(unsigned int vertexCapacity, ACTION_IF_BUSY action);
	unsigned int AppendVertices(Vertex* vertices, unsigned int length, D3D11_PRIMITIVE_TOPOLOGY primitiveType);
	void commit();
	void DeleteElement(unsigned int index);
	void Render();
private:
	struct RenderElement {
		unsigned int startIndex, length;
		D3D11_PRIMITIVE_TOPOLOGY primitiveType;
	};

	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;
	std::vector<RenderElement> elements;
	ID3D11Buffer* buffer;
	Vertex* newVertices;
	unsigned int currentIndex, maxIndex, newVertexCount, indexAtLock;
	bool locked;
};