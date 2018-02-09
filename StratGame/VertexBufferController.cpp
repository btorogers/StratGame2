#include "VertexBufferController.h"

VertexBufferController::VertexBufferController(ID3D11Device* dev, ID3D11DeviceContext* devcon) {
	this->dev = dev;
	this->devcon = devcon;
	bufferSize = 1024;
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
	bufferDesc.ByteWidth = sizeof(unsigned int) * bufferSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&bufferDesc, NULL, &indexBuffer);
}

VertexBufferController::~VertexBufferController() {
	vertexBuffer->Release();
	indexBuffer->Release();
	elements.clear();
	elements.resize(0);
}

bool VertexBufferController::lock(unsigned int vertexCapacity, unsigned int indexCapacity, ACTION_IF_BUSY action) {
	if (locked) {
		if (action == RETURN_FALSE) {
			return false;
		} else {
			while (locked) {
				Sleep(2);
			}
		}
	}

	if (currentIndex + vertexCapacity > bufferSize || elements.size() + indexCapacity > bufferSize) {
		bufferSize *= 2;

		/*std::stringstream stream;
		stream << bufferSize << std::endl;
		stream << currentIndex << std::endl;
		stream << elements.size() << std::endl;
		stream << elements.max_size() << std::endl;
		MessageBox(NULL, stream.str().c_str(), "hi", MB_OK);*/

		elements.reserve(bufferSize);

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof(Vertex) * bufferSize;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ID3D11Buffer* newBuffer;
		dev->CreateBuffer(&bufferDesc, NULL, &newBuffer);

		D3D11_BOX box = { 0, 0, 0, currentIndex * sizeof(Vertex), 1, 1 };
		devcon->CopySubresourceRegion(newBuffer, 0, 0, 0, 0, vertexBuffer, 0, &box);

		vertexBuffer->Release();
		vertexBuffer = newBuffer;
		indexBuffer->Release();
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof(unsigned int) * bufferSize;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		dev->CreateBuffer(&bufferDesc, NULL, &indexBuffer);
	}

	newVertices = new Vertex[vertexCapacity];
	indexAtLock = currentIndex;
	newVertexCount = 0;
	locked = true;

	return true;
}

unsigned int VertexBufferController::AppendPrimitive(Vertex* vertices) {
	if (!locked) {
		throw std::invalid_argument("Lock buffer before writing data");
	}
	elements.push_back(currentIndex++);
	elements.push_back(currentIndex++);
	elements.push_back(currentIndex++);

	memcpy(newVertices + newVertexCount, vertices, sizeof(Vertex) * 3);

	newVertexCount += 3;

	return elements.size() - 4;
}

unsigned int VertexBufferController::AppendVertices(Vertex* vertices, int* indices, unsigned int lv, unsigned int li) {
	if (!locked) {
		throw std::invalid_argument("Lock buffer before writing data");
	}
	for (unsigned int x = 0; x < li; x++) {
		elements.push_back(indices[x] + currentIndex);
	}

	memcpy(newVertices + newVertexCount, vertices, sizeof(Vertex) * lv);

	currentIndex += lv;
	newVertexCount += lv;

	return elements.size() - li;
}

void VertexBufferController::commit() {
	D3D11_MAPPED_SUBRESOURCE mapSub;
	devcon->Map(vertexBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, NULL, &mapSub);
	memcpy((Vertex*)mapSub.pData + indexAtLock, newVertices, sizeof(Vertex) * newVertexCount);
	devcon->Unmap(vertexBuffer, 0);

	updateIndices();

	delete[] newVertices;
	locked = false;
}

void VertexBufferController::DeletePrimitive(unsigned int index) {
	elements.erase(elements.begin() + index, elements.begin() + index + 3);
	updateIndices();
}

void VertexBufferController::Render() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(elements.size(), 0, 0);
}

void VertexBufferController::updateIndices() {
	D3D11_MAPPED_SUBRESOURCE mapSub;
	devcon->Map(indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mapSub);
	memcpy(mapSub.pData, &elements.front(), sizeof(unsigned int) * elements.size());
	devcon->Unmap(indexBuffer, 0);
}
