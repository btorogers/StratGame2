#pragma once

#include "Includes.h"
#include "Vertex.h"

class VertexBufferController {
public:
	VertexBufferController(ID3D11Device* dev, ID3D11DeviceContext* devcon, std::mutex* devconlock);
	~VertexBufferController();

	void GenerateGrid(int sizeX, int sizeY);

	int lock(int vertexCapacity, int indexCapacity, bool dynamic);
	void AppendPrimitive(Vertex* vertices);
	void AppendVertices(Vertex* vertices, int* indices, int lengthVertices, int lengthIndices);
	void commit();

	void DeletePrimitive(int index, bool dynamic);

	void RenderStatic();
	void RenderDynamic(int startIndex, int vertexCount);
private:
	void UpdateIndices();

	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;
	ID3D11Buffer* vertexBuffer,* staticIndexBuffer,* dynamicIndexBuffer;
	Vertex* newVertices;
	std::mutex* devconlock;

	std::vector<int> staticElements, dynamicElements;
	int currentIndex, bufferSize, newVertexCount, indexAtLock, gridVertexLength;
	bool locked, dynamicLock;
};