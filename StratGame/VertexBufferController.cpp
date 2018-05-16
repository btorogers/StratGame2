#include "VertexBufferController.h"

VertexBufferController::VertexBufferController(ID3D11Device* dev, ID3D11DeviceContext* devcon, std::mutex* devconlock): dev(dev), devcon(devcon), devconlock(devconlock) {
	// initial buffer size for all buffers, should split this up since vertex will be like 1000x the size of instance
	bufferSize = 8192;
	currentIndex = newVertexCount = 0;
	locked = false;

	D3D11_BUFFER_DESC bufferDesc;
	// create the vertex buffer
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(Vertex) * bufferSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&bufferDesc, NULL, &vertexBuffer);

	// create the instance buffer
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(ObjectData) * bufferSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&bufferDesc, NULL, &instanceBuffer);

	// create the index buffers
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(int) * bufferSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// create 2 buffers with the same description
	dev->CreateBuffer(&bufferDesc, NULL, &staticIndexBuffer);
	dev->CreateBuffer(&bufferDesc, NULL, &dynamicIndexBuffer);
}

VertexBufferController::~VertexBufferController() {
	vertexBuffer->Release();
	instanceBuffer->Release();
	staticIndexBuffer->Release();
	dynamicIndexBuffer->Release();
	staticElements.clear();
	dynamicElements.clear();
}

void VertexBufferController::GenerateGrid(int sizeX, int sizeY) {
	// each line needs 2 vertices, 2 directions, 4 for corners
	gridVertexLength = 2 * sizeX + 2 * sizeY + 4;

	newVertices = new Vertex[gridVertexLength];
	// fill the first half of the array with vertical lines
	for (int x = 0; x <= 2 * sizeX; x += 2) {
		newVertices[x].position = D3DXVECTOR3((float)x / 2, 0, 0);
		newVertices[x + 1].position = D3DXVECTOR3((float)x / 2, 0, (float)sizeY);
		newVertices[x].color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		newVertices[x].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		newVertices[x + 1].color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		newVertices[x + 1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	// fill the second half of the array with horizontal lines
	int offset = 2 * sizeX + 2;
	for (int y = 0; y <= 2 * sizeY; y += 2) {
		newVertices[offset + y].position = D3DXVECTOR3(0, 0, y / 2.0f);
		newVertices[offset + y + 1].position = D3DXVECTOR3((float)sizeX, 0, y / 2.0f);
		newVertices[offset + y].color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		newVertices[offset + y].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		newVertices[offset + y + 1].color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		newVertices[offset + y + 1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	indexAtLock = 0;
	currentIndex = gridVertexLength;

	D3D11_MAPPED_SUBRESOURCE mapSub;
	devconlock->lock();
	devcon->Map(vertexBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, NULL, &mapSub);
	memcpy((Vertex*)mapSub.pData + indexAtLock, newVertices, sizeof(Vertex) * gridVertexLength);
	devcon->Unmap(vertexBuffer, 0);
	devconlock->unlock();
	delete[] newVertices;
}

int VertexBufferController::lock(int vertexCapacity, int indexCapacity, bool dynamic) {
	if (locked) {
		while (locked) {
			Sleep(2);
		}
	}
	locked = true;

	// if any of the buffers wont be able to support the new object, increase the size
	if (currentIndex + vertexCapacity > bufferSize || (int)staticElements.size() + indexCapacity > bufferSize
		|| (int)dynamicElements.size() + indexCapacity > bufferSize) {
		bufferSize *= 2;

		/*std::stringstream stream;
		stream << "doubling buffer" << std::endl;
		OutputDebugString(stream.str().c_str());*/

		staticElements.reserve(bufferSize);

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof(Vertex) * bufferSize;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ID3D11Buffer* newBuffer;
		dev->CreateBuffer(&bufferDesc, NULL, &newBuffer);
		
		D3D11_BOX box = { 0, 0, 0, currentIndex * sizeof(Vertex), 1, 1 };
		devconlock->lock();
		devcon->CopySubresourceRegion(newBuffer, 0, 0, 0, 0, vertexBuffer, 0, &box);
		devconlock->unlock();

		vertexBuffer->Release();
		vertexBuffer = newBuffer;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof(int) * bufferSize;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		staticIndexBuffer->Release();
		dev->CreateBuffer(&bufferDesc, NULL, &staticIndexBuffer);

		dynamicIndexBuffer->Release();
		dev->CreateBuffer(&bufferDesc, NULL, &dynamicIndexBuffer);

		UpdateIndices();
		dynamicLock = !dynamicLock;
		UpdateIndices();
		dynamicLock = !dynamicLock;
	}

	newVertices = new Vertex[vertexCapacity];
	indexAtLock = currentIndex;
	newVertexCount = 0;
	dynamicLock = dynamic;

	// return the index where the new vertices will be placed
	return (dynamicLock ? dynamicElements.size() : staticElements.size());
}

void VertexBufferController::AppendPrimitive(Vertex* vertices) {
if (!locked) {
	throw std::invalid_argument("Lock buffer before writing data");
}
if (dynamicLock) {
	dynamicElements.push_back(currentIndex++);
	dynamicElements.push_back(currentIndex++);
	dynamicElements.push_back(currentIndex++);
}
else {
	staticElements.push_back(currentIndex++);
	staticElements.push_back(currentIndex++);
	staticElements.push_back(currentIndex++);
}

memcpy(newVertices + newVertexCount, vertices, sizeof(Vertex) * 3);

newVertexCount += 3;
}

void VertexBufferController::AppendVertices(Vertex* vertices, int* indices, int lv, int li) {
	if (!locked) {
		throw std::invalid_argument("Lock buffer before writing data");
	}
	for (int x = 0; x < li; x++) {
		if (dynamicLock) {
			dynamicElements.push_back(indices[x] + currentIndex);
		}
		else {
			staticElements.push_back(indices[x] + currentIndex);
		}
	}

	memcpy(newVertices + newVertexCount, vertices, sizeof(Vertex) * lv);

	currentIndex += lv;
	newVertexCount += lv;
}

void VertexBufferController::commit() {
	D3D11_MAPPED_SUBRESOURCE mapSub;
	devconlock->lock();
	devcon->Map(vertexBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, NULL, &mapSub);
	memcpy((Vertex*)mapSub.pData + indexAtLock, newVertices, sizeof(Vertex) * newVertexCount);
	devcon->Unmap(vertexBuffer, 0);
	devconlock->unlock();

	UpdateIndices();

	delete[] newVertices;
	locked = false;
}

int* VertexBufferController::AddInstance(ObjectData instance) {
	return AddMultipleInstances(&instance, 1);
	/*instances.push_back(instance);
	// also keep track of what indices we give out so they can be updated when an instance is deleted
	instanceIndices.push_back(new int{ (int)instances.size() - 1 });
	UpdateInstances();
	return instanceIndices.back();*/
}

void VertexBufferController::DeleteInstance(int* instanceIndex) {
	return DeleteMultipleInstances(instanceIndex, 1);
	/*instances.erase(instances.begin() + *instanceIndex);
	// iterate over indices of instances to delete given index and reduce value of subsequent ones
	bool found = false;
	for (auto it = instanceIndices.begin(), end = instanceIndices.end(); it != end; it++) {
		if (found) {
			// dereference iterator to get an int pointer dereference that to reduce int value by 1
			(*(*it))--;
		}
		else if (*it == instanceIndex) {
			/*it = instanceIndices.erase(it);
			if (it != end) {
				(*(*it))--;
			}* /
			found = true;
		}
	}
	// remove the instance index pointer from the list
	for (auto it = instanceIndices.begin(), end = instanceIndices.end(); it != end; it++) {
		if (*it == instanceIndex) {
			it = instanceIndices.erase(it);
			break;
		}
	}
	UpdateInstances();*/
}

void VertexBufferController::UpdateInstance(ObjectData instance, int instanceIndex) {
	return UpdateMultipleInstances(&instance, 1, instanceIndex);
	/*instances.at(instanceIndex) = instance;
	UpdateInstances();*/
}

int* VertexBufferController::AddMultipleInstances(ObjectData* instancesStart, int instanceCount) {
	instanceIndices.push_back(new int{ (int)instances.size() });

	instances.insert(instances.end(), instancesStart, instancesStart + instanceCount);

	UpdateInstances();

	return instanceIndices.back();
}

void VertexBufferController::DeleteMultipleInstances(int* instancesStartIndex, int instanceCount) {
	int instanceEndIndex = *instancesStartIndex + instanceCount;

	instances.erase(instances.begin() + *instancesStartIndex, instances.begin() + instanceEndIndex);

	// decrease value of all subsequent instance indices
	bool found = false;
	for (auto it = instanceIndices.begin(), end = instanceIndices.end(); it != end; it++) {
		if (found) {
			// dereference iterator to get an int pointer dereference that to reduce int value
			(*(*it)) -= instanceCount;
		}
		else if (*it == instancesStartIndex) {
			// record that the instance index has been found, so consecutive iterations will decrease value
			found = true;
		}
	}

	// remove the instance index pointer
	instanceIndices.erase(std::find(instanceIndices.begin(), instanceIndices.end(), instancesStartIndex));
	
	UpdateInstances();
}

void VertexBufferController::UpdateMultipleInstances(ObjectData* instancesStart, int instanceCount, int instanceStartIndex) {
	memcpy(&instances[instanceStartIndex], instancesStart, instanceCount * sizeof(ObjectData));
	UpdateInstances();
}

void VertexBufferController::DeletePrimitive(int index, bool dynamic) {
	dynamicLock = dynamic;
	if (dynamicLock) {
		dynamicElements.erase(dynamicElements.begin() + index, dynamicElements.begin() + index + 3);
	} else {
		staticElements.erase(staticElements.begin() + index, staticElements.begin() + index + 3);
	}
	
	UpdateInstances();
}

void VertexBufferController::RenderStatic() {
	static UINT stride = sizeof(Vertex);
	static UINT offset = 0;
	devconlock->lock();
	devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// draw the grid
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	devcon->Draw(gridVertexLength, 0);

	devcon->IASetIndexBuffer(staticIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(staticElements.size(), 0, 0);
	devconlock->unlock();
}

void VertexBufferController::RenderInstanced(int startIndex, int indexCount, int instanceIndex, int instanceCount) {
	static UINT stride[2] = { sizeof(Vertex), sizeof(ObjectData) };
	static UINT offset[2] = { 0, 0 };
	static ID3D11Buffer* buffers[2] = { vertexBuffer, instanceBuffer };
	devconlock->lock();
	devcon->IASetVertexBuffers(0, 2, buffers, stride, offset);
	devcon->IASetIndexBuffer(dynamicIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//devcon->DrawIndexed(indexCount, startIndex, 0);
	devcon->DrawIndexedInstanced(indexCount, instanceCount, startIndex, 0, instanceIndex);
	devconlock->unlock();
}

void VertexBufferController::UpdateIndices() {
	D3D11_MAPPED_SUBRESOURCE mapSub;
	devconlock->lock();
	if (dynamicLock) {
		devcon->Map(dynamicIndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mapSub);
		memcpy(mapSub.pData, &dynamicElements.front(), sizeof(int) * dynamicElements.size());
		devcon->Unmap(dynamicIndexBuffer, 0);
	} else {
		devcon->Map(staticIndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mapSub);
		memcpy(mapSub.pData, &staticElements.front(), sizeof(int) * staticElements.size());
		devcon->Unmap(staticIndexBuffer, 0);
	}
	devconlock->unlock();
}

void VertexBufferController::UpdateInstances() {
	D3D11_MAPPED_SUBRESOURCE mapSub;
	devconlock->lock();
	devcon->Map(instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mapSub);
	// check instances has data and therefore can be accessed without exception
	if (instances.size())
		memcpy(mapSub.pData, &instances.front(), sizeof(ObjectData) * instances.size());
	devcon->Unmap(instanceBuffer, 0);
	devconlock->unlock();
}