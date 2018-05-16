#pragma once

#include "Includes.h"
#include "Vertex.h"
#include "ObjectData.h"

/*#define AddInstance(instance) AddMultipleInstances(&instance, 1)
#define DeleteInstance(instanceIndex) DeleteMultipleInstances(instanceIndex, 1)
#define UpdateInstance(instance, instanceIndex) UpdateMultipleInstances(&instance, 1, instanceIndex)*/

class VertexBufferController {
public:
	VertexBufferController(ID3D11Device* dev, ID3D11DeviceContext* devcon, std::mutex* devconlock);
	~VertexBufferController();

	void GenerateGrid(int sizeX, int sizeY);

	int lock(int vertexCapacity, int indexCapacity, bool dynamic);
	void AppendPrimitive(Vertex* vertices);
	void AppendVertices(Vertex* vertices, int* indices, int lengthVertices, int lengthIndices);
	void commit();

	int* AddInstance(ObjectData instance);
	void DeleteInstance(int* instanceIndex);
	void UpdateInstance(ObjectData instance, int instanceIndex);

	int* AddMultipleInstances(ObjectData* instancesStart, int instanceCount);
	void DeleteMultipleInstances(int* instancesStartIndex, int instanceCount);
	void UpdateMultipleInstances(ObjectData* instancesStart, int instanceCount, int instanceStartIndex);

	void DeletePrimitive(int index, bool dynamic);

	void RenderStatic();
	void RenderInstanced(int startIndex, int indexCount, int instanceIndex, int instanceCount);
private:
	void UpdateIndices();
	void UpdateInstances();

	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;
	ID3D11Buffer* vertexBuffer, *instanceBuffer, *staticIndexBuffer, *dynamicIndexBuffer;
	Vertex* newVertices;
	std::mutex* devconlock;

	std::vector<int> staticElements, dynamicElements;
	std::vector<ObjectData> instances;
	std::vector<int*> instanceIndices;
	int currentIndex, bufferSize, newVertexCount, indexAtLock, gridVertexLength;
	bool locked, dynamicLock;
};