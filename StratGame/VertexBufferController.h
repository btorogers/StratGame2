#pragma once

#include "Includes.h"
#include "Vertex.h"

class VertexBufferController {
public:
	VertexBufferController(ID3D11Device* dev, ID3D11DeviceContext* devcon);
	~VertexBufferController();
	int lock(int vertexCapacity, int indexCapacity, bool dynamic);
	void AppendPrimitive(Vertex* vertices);
	void AppendVertices(Vertex* vertices, int* indices, int lengthVertices, int lengthIndices);
	void commit();
	void DeletePrimitive(int index, bool dynamic);
	void RenderStatic();
	void RenderDynamic(int startIndex, int vertexCount);
private:
	void updateIndices();

	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;
	std::vector<int> staticElements, dynamicElements;
	ID3D11Buffer* vertexBuffer,* staticIndexBuffer,* dynamicIndexBuffer;
	Vertex* newVertices;
	int currentIndex, bufferSize, newVertexCount, indexAtLock;
	bool locked, dynamicLock;
};