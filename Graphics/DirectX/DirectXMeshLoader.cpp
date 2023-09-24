#include "DirectXMeshLoader.h"

#include "../Data/Vertex.h"

#include "../Utils.h"
#include <d3dx11effect.h>

const leap::graphics::DirectXMeshLoader::DirectXMeshDefinition& leap::graphics::DirectXMeshLoader::LoadMesh(const std::string& dataPath, ID3D11Device* pDevice)
{
	if (auto it{ m_Meshes.find(dataPath) }; it != end(m_Meshes))
	{
		return it->second;
	}

	DirectXMeshDefinition mesh{};

	std::vector<Vertex> vertices{};
	std::vector<unsigned int> indices{};

	Utils::ParseObj(dataPath, vertices, indices);

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

	m_Meshes[dataPath] = mesh;

	return m_Meshes[dataPath];
}

leap::graphics::DirectXMeshLoader::~DirectXMeshLoader()
{
	for (auto& mesh : m_Meshes)
	{
		if (mesh.second.vertexBuffer) mesh.second.vertexBuffer->Release();
		if (mesh.second.indexBuffer) mesh.second.indexBuffer->Release();
	}
}
