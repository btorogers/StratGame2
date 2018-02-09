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
	bool lock(unsigned int vertexCapacity, unsigned int indexCapacity, ACTION_IF_BUSY action);
	unsigned int AppendPrimitive(Vertex* vertices);
	unsigned int AppendVertices(Vertex* vertices, int* indices, unsigned int lengthVertices, unsigned int lengthIndices);
	void commit();
	void DeletePrimitive(unsigned int index);
	void Render();
private:
	void updateIndices();

	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;
	std::vector<unsigned int> elements;
	ID3D11Buffer* vertexBuffer,* indexBuffer;
	Vertex* newVertices;
	unsigned int currentIndex, bufferSize, newVertexCount, indexAtLock;
	bool locked;
};