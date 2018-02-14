#include "VertexBufferController.h"

// good luck remembering how this works in future lol

VertexBufferController::VertexBufferController(ID3D11Device* dev, ID3D11DeviceContext* devcon): dev(dev), devcon(devcon) {
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

int VertexBufferController::lock(int vertexCapacity, int indexCapacity, bool dynamic) {
	if (locked) {
		while (locked) {
			Sleep(2);
		}
	}
	locked = true;

	if (currentIndex + vertexCapacity > bufferSize || (int)staticElements.size() + indexCapacity > bufferSize) {
		bufferSize *= 2;

		/*std::stringstream stream;
		stream << bufferSize << std::endl;
		stream << currentIndex << std::endl;
		stream << staticElements.size() << std::endl;
		stream << staticElements.max_size() << std::endl;
		MessageBox(NULL, stream.str().c_str(), "hi", MB_OK);*/

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
		devcon->CopySubresourceRegion(newBuffer, 0, 0, 0, 0, vertexBuffer, 0, &box);

		vertexBuffer->Release();
		vertexBuffer = newBuffer;
		staticIndexBuffer->Release();
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof(int) * bufferSize;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		dev->CreateBuffer(&bufferDesc, NULL, &staticIndexBuffer);

		dynamicIndexBuffer->Release();
		dev->CreateBuffer(&bufferDesc, NULL, &dynamicIndexBuffer);
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
	devcon->Map(vertexBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, NULL, &mapSub);
	memcpy((Vertex*)mapSub.pData + indexAtLock, newVertices, sizeof(Vertex) * newVertexCount);
	devcon->Unmap(vertexBuffer, 0);

	updateIndices();

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
	
	updateIndices();
}

void VertexBufferController::RenderStatic() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(staticIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(staticElements.size(), 0, 0);
}

void VertexBufferController::RenderDynamic(int startIndex, int indexCount) {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(dynamicIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->DrawIndexed(indexCount, startIndex, 0);
}

void VertexBufferController::updateIndices() {
	D3D11_MAPPED_SUBRESOURCE mapSub;
	if (dynamicLock) {
		devcon->Map(dynamicIndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mapSub);
		memcpy(mapSub.pData, &dynamicElements.front(), sizeof(int) * dynamicElements.size());
		devcon->Unmap(dynamicIndexBuffer, 0);
	} else {
		devcon->Map(staticIndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mapSub);
		memcpy(mapSub.pData, &staticElements.front(), sizeof(int) * staticElements.size());
		devcon->Unmap(staticIndexBuffer, 0);
	}
}
