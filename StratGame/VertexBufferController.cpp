#include "VertexBufferController.h"

// good luck remembering how this works in future lol

VertexBufferController::VertexBufferController(ID3D11Device* dev, ID3D11DeviceContext* devcon, std::mutex* devconlock): dev(dev), devcon(devcon), devconlock(devconlock) {
	bufferSize = 8192;
	currentIndex = newVertexCount = 0;
	locked = false;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(Vertex) * bufferSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&bufferDesc, NULL, &vertexBuffer);

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(int) * bufferSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&bufferDesc, NULL, &staticIndexBuffer);
	dev->CreateBuffer(&bufferDesc, NULL, &dynamicIndexBuffer);
}

VertexBufferController::~VertexBufferController() {
	vertexBuffer->Release();
	staticIndexBuffer->Release();
	dynamicIndexBuffer->Release();
	staticElements.clear();
	dynamicElements.clear();
}

void VertexBufferController::GenerateGrid(int sizeX, int sizeY) {
	gridVertexLength = 2 * sizeX + 2 * sizeY + 4;

	newVertices = new Vertex[gridVertexLength];
	for (int x = 0; x < sizeX + 1; x++) {
		newVertices[2 * x].position = D3DXVECTOR3((float)x, 0, 0);
		newVertices[2 * x + 1].position = D3DXVECTOR3((float)x, 0, (float)sizeY);
		newVertices[2 * x].color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		newVertices[2 * x].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		newVertices[2 * x + 1].color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		newVertices[2 * x + 1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}
	int offset = 2 * sizeX + 2;
	for (int y = 0; y < sizeY + 1; y++) {
		newVertices[offset + 2 * y].position = D3DXVECTOR3(0, 0, (float)y);
		newVertices[offset + 2 * y + 1].position = D3DXVECTOR3((float)sizeX, 0, (float)y);
		newVertices[offset + 2 * y].color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		newVertices[offset + 2 * y].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		newVertices[offset + 2 * y + 1].color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		newVertices[offset + 2 * y + 1].normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
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
	} else {
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
		} else {
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

void VertexBufferController::DeletePrimitive(int index, bool dynamic) {
	dynamicLock = dynamic;
	if (dynamicLock) {
		dynamicElements.erase(dynamicElements.begin() + index, dynamicElements.begin() + index + 3);
	} else {
		staticElements.erase(staticElements.begin() + index, staticElements.begin() + index + 3);
	}
	
	UpdateIndices();
}

void VertexBufferController::RenderStatic() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devconlock->lock();
	devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	devcon->Draw(gridVertexLength, 0);

	devcon->IASetIndexBuffer(staticIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(staticElements.size(), 0, 0);
	devconlock->unlock();
}

void VertexBufferController::RenderDynamic(int startIndex, int indexCount) {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devconlock->lock();
	devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(dynamicIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(indexCount, startIndex, 0);
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
