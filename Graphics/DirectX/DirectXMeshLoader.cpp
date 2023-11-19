#include "DirectXMeshLoader.h"

#include "../MeshLoader.h"
#include "../Data/CustomMesh.h"

#include <d3dx11effect.h>

const leap::graphics::DirectXMeshLoader::DirectXMeshDefinition& leap::graphics::DirectXMeshLoader::LoadMesh(const std::string& dataPath, ID3D11Device* pDevice)
{
	if (auto it{ m_Meshes.find(dataPath) }; it != end(m_Meshes))
	{
		return it->second;
	}

	DirectXMeshDefinition mesh{ CreateMesh(dataPath, pDevice) };

	m_Meshes[dataPath] = mesh;

	return m_Meshes[dataPath];
}

const leap::graphics::DirectXMeshLoader::DirectXMeshDefinition& leap::graphics::DirectXMeshLoader::LoadMesh(const CustomMesh& mesh, ID3D11Device* pDevice)
{
	DirectXMeshDefinition directXMesh{ CreateMesh(mesh.GetVertexBuffer(), mesh.GetVertexSize(), mesh.GetIndexBuffer(), pDevice)};

	m_CustomMeshes.push_back(directXMesh);

	return m_CustomMeshes[m_CustomMeshes.size() - 1];
}

void leap::graphics::DirectXMeshLoader::RemoveCustomMesh(ID3D11Buffer* pVertexBuffer)
{
	auto meshIt{ std::find_if(begin(m_CustomMeshes), end(m_CustomMeshes), [pVertexBuffer](const auto& mesh) { return mesh.vertexBuffer == pVertexBuffer; }) };
	meshIt->vertexBuffer->Release();
	meshIt->indexBuffer->Release();
	m_CustomMeshes.erase(meshIt);
}

leap::graphics::DirectXMeshLoader::~DirectXMeshLoader()
{
	for (auto& mesh : m_Meshes)
	{
		if (mesh.second.vertexBuffer) mesh.second.vertexBuffer->Release();
		if (mesh.second.indexBuffer) mesh.second.indexBuffer->Release();
	}
	for (auto& mesh : m_CustomMeshes)
	{
		if (mesh.vertexBuffer) mesh.vertexBuffer->Release();
		if (mesh.indexBuffer) mesh.indexBuffer->Release();
	}
}

leap::graphics::DirectXMeshLoader::DirectXMeshDefinition leap::graphics::DirectXMeshLoader::CreateMesh(const std::string& dataPath, ID3D11Device* pDevice) const
{
	std::vector<Vertex> vertices{};
	std::vector<unsigned int> indices{};

	MeshLoader::ParseObj(dataPath, vertices, indices);

	return CreateMesh(vertices, indices, pDevice);
}

leap::graphics::DirectXMeshLoader::DirectXMeshDefinition leap::graphics::DirectXMeshLoader::CreateMesh(const std::vector<Vertex> vertices, const std::vector<unsigned int> indices, ID3D11Device* pDevice) const
{
	DirectXMeshDefinition mesh{};

	mesh.nrIndices = static_cast<unsigned int>(indices.size());

	// Create vertex buffer
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(Vertex) * static_cast<unsigned int>(vertices.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertices.data();

	HRESULT result{ pDevice->CreateBuffer(&bd, &initData, &mesh.vertexBuffer) };

	// Create index buffer
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(unsigned int) * static_cast<unsigned int>(indices.size());
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = indices.data();

	result = pDevice->CreateBuffer(&bd, &initData, &mesh.indexBuffer);

	mesh.vertexSize = sizeof(Vertex);

	return mesh;
}

leap::graphics::DirectXMeshLoader::DirectXMeshDefinition leap::graphics::DirectXMeshLoader::CreateMesh(const std::vector<unsigned char>& vertexData, unsigned int vertexSize, const std::vector<unsigned int>& indices, ID3D11Device* pDevice) const
{
	DirectXMeshDefinition mesh{};

	mesh.nrIndices = static_cast<unsigned int>(indices.size());
	mesh.vertexSize = vertexSize;

	// Create vertex buffer
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = static_cast<unsigned int>(vertexData.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertexData.data();

	HRESULT result{ pDevice->CreateBuffer(&bd, &initData, &mesh.vertexBuffer) };

	// Create index buffer
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(unsigned int) * static_cast<unsigned int>(indices.size());
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = indices.data();

	result = pDevice->CreateBuffer(&bd, &initData, &mesh.indexBuffer);

	return mesh;
}
