#include "VertexBufferController.h"

VertexBufferController::VertexBufferController(ID3D11Device* dev, ID3D11DeviceContext* devcon) {
	this->dev = dev;
	this->devcon = devcon;
	maxVert = 256;
	maxIndex = 0;

	D3D11_BUFFER_DESC vertexbd;
	ZeroMemory(&vertexbd, sizeof(vertexbd));

	vertexbd.Usage = D3D11_USAGE_DYNAMIC;
	vertexbd.ByteWidth = sizeof(Vertex) * maxVert;
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

unsigned int VertexBufferController::AppendVertices(Vertex *vertices, unsigned int length, D3D11_PRIMITIVE_TOPOLOGY primitiveType) {
	RenderElement e;
	e.length = length;
	e.startIndex = maxIndex;
	e.primitiveType = primitiveType;
	elements.push_back(e);

	if (maxIndex + length > maxVert) {
		maxVert *= 2;

		/*std::stringstream stream;
		stream << maxVert;
		MessageBox(NULL, stream.str().c_str(), "hi", MB_OK);*/

		D3D11_BUFFER_DESC newbd;
		ZeroMemory(&newbd, sizeof(newbd));

		newbd.Usage = D3D11_USAGE_DYNAMIC;
		newbd.ByteWidth = sizeof(Vertex) * maxVert;
		newbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		newbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ID3D11Buffer* newBuffer;
		dev->CreateBuffer(&newbd, NULL, &newBuffer);

		D3D11_BOX box = { 0, 0, 0, maxIndex * sizeof(Vertex), 1, 1 };
		devcon->CopySubresourceRegion(newBuffer, 0, 0, 0, 0, buffer, 0, &box);

		buffer->Release();
		buffer = newBuffer;
	}

	D3D11_MAPPED_SUBRESOURCE mapSub;
	devcon->Map(buffer, NULL, D3D11_MAP_WRITE_NO_OVERWRITE, NULL, &mapSub);
	memcpy((Vertex*)mapSub.pData + maxIndex, vertices, sizeof(Vertex)*length);
	devcon->Unmap(buffer, NULL);

	maxIndex += length;

	return maxIndex - length;
}

void VertexBufferController::DeleteElement(unsigned int index) {

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
