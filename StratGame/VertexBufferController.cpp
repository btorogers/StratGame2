#include "VertexBufferController.h"

VertexBufferController::VertexBufferController(ID3D11Device* dev, ID3D11DeviceContext* devcon) {
	this->dev = dev;
	this->devcon = devcon;
	maxIndex = 256;
	currentIndex = 0;
	locked = false;
	newVertexCount = 0;

	D3D11_BUFFER_DESC vertexbd;
	ZeroMemory(&vertexbd, sizeof(vertexbd));

	vertexbd.Usage = D3D11_USAGE_DYNAMIC;
	vertexbd.ByteWidth = sizeof(Vertex) * maxIndex;
	vertexbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	dev->CreateBuffer(&vertexbd, NULL, &buffer);
}

VertexBufferController::~VertexBufferController() {
	buffer->Release();
	buffer = 0;
	elements.clear();
	elements.resize(0);
}

bool VertexBufferController::lock(unsigned int vertexCapacity, ACTION_IF_BUSY action) {
	if (locked) {
		if (action == RETURN_FALSE) {
			return false;
		} else {
			while (locked) {
				Sleep(2);
			}
		}
	}

	if (currentIndex + vertexCapacity > maxIndex) {
		maxIndex *= 2;

		/*std::stringstream stream;
		stream << maxIndex;
		MessageBox(NULL, stream.str().c_str(), "hi", MB_OK);*/

		D3D11_BUFFER_DESC newbd;
		ZeroMemory(&newbd, sizeof(newbd));

		newbd.Usage = D3D11_USAGE_DYNAMIC;
		newbd.ByteWidth = sizeof(Vertex) * maxIndex;
		newbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		newbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ID3D11Buffer* newBuffer;
		dev->CreateBuffer(&newbd, NULL, &newBuffer);

		D3D11_BOX box = { 0, 0, 0, currentIndex * sizeof(Vertex), 1, 1 };
		devcon->CopySubresourceRegion(newBuffer, 0, 0, 0, 0, buffer, 0, &box);

		buffer->Release();
		buffer = newBuffer;
	}

	newVertices = new Vertex[vertexCapacity];
	indexAtLock = currentIndex;
	newVertexCount = 0;
	locked = true;

	return true;
}

unsigned int VertexBufferController::AppendVertices(Vertex* vertices, unsigned int length, D3D11_PRIMITIVE_TOPOLOGY primitiveType) {
	if (!locked) {
		throw std::invalid_argument("Lock buffer before writing data");
	}
	RenderElement e;
	e.length = length;
	e.startIndex = currentIndex;
	e.primitiveType = primitiveType;
	elements.push_back(e);

	memcpy(newVertices + newVertexCount, vertices, sizeof(Vertex) * length);

	currentIndex += length;
	newVertexCount += length;

	return elements.size() - 1;
}

void VertexBufferController::commit() {
	D3D11_MAPPED_SUBRESOURCE mapSub;
	devcon->Map(buffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, NULL, &mapSub);
	memcpy((Vertex*)mapSub.pData + indexAtLock, newVertices, sizeof(Vertex) * newVertexCount);
	devcon->Unmap(buffer, 0);

	delete[] newVertices;
	locked = false;
}

void VertexBufferController::DeleteElement(unsigned int index) {
	elements.erase(elements.begin() + index);

}

void VertexBufferController::Render() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);

	/*for (unsigned int x = 0; x < elements.size(); x++) {
		devcon->IASetPrimitiveTopology(elements.at(x).primitiveType);
		devcon->Draw(elements.at(x).length, elements.at(x).startIndex);
	}*/

	for (RenderElement e : elements) {
		devcon->IASetPrimitiveTopology(e.primitiveType);
		devcon->Draw(e.length, e.startIndex);
	}
}
