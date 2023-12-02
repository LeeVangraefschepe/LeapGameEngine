#include "DirectXMesh.h"

#include "DirectXEngine.h"

#include "../Data/Vertex.h"
#include "../Data/CustomMesh.h"
#include "../MeshLoader.h"

#include <vector>

#include <d3d11.h>

#include <Debug.h>

leap::graphics::DirectXMesh::DirectXMesh(DirectXEngine* pEngine)
	: m_pEngine{ pEngine }
{
}

leap::graphics::DirectXMesh::DirectXMesh(DirectXEngine* pEngine, const std::string& filePath)
	: DirectXMesh(pEngine)
{
	std::vector<Vertex> vertices{};
	std::vector<unsigned int> indices{};

	// Load obj
	MeshLoader::ParseObj(filePath, vertices, indices);

	// Set index amoutn and vertex size
	m_NrIndices = static_cast<unsigned int>(indices.size());
	m_VertexSize = sizeof(Vertex);

	// Create vertex buffer
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(Vertex) * static_cast<unsigned int>(vertices.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertices.data();

	HRESULT result{ pEngine->GetDevice()->CreateBuffer(&bd, &initData, &m_pVertexBuffer)};
	if (FAILED(result))
	{
		Debug::LogError("DirectXEngine Error : Failed to create vertex buffer from obj");
		return;
	}

	// Create index buffer
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(unsigned int) * static_cast<unsigned int>(indices.size());
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = indices.data();

	result = pEngine->GetDevice()->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	if (FAILED(result)) Debug::LogError("DirectXEngine Error : Failed to create index buffer from obj");
}

leap::graphics::DirectXMesh::~DirectXMesh()
{
	Release();
}

void leap::graphics::DirectXMesh::ReloadMesh(const CustomMesh& mesh)
{
	Release();

	// Set index amoutn and vertex size
	m_NrIndices = static_cast<unsigned int>(mesh.GetIndexBuffer().size());
	m_VertexSize = mesh.GetVertexSize();

	// Create vertex buffer
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = static_cast<unsigned>(mesh.GetVertexBuffer().size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = mesh.GetVertexBuffer().data();

	HRESULT result{ m_pEngine->GetDevice()->CreateBuffer(&bd, &initData, &m_pVertexBuffer)};
	if (FAILED(result))
	{
		Debug::LogError("DirectXEngine Error : Failed to create vertex buffer from obj");
		return;
	}

	// Create index buffer
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(unsigned int) * static_cast<unsigned int>(m_NrIndices);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = mesh.GetIndexBuffer().data();

	result = m_pEngine->GetDevice()->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	if (FAILED(result)) Debug::LogError("DirectXEngine Error : Failed to create index buffer from obj");
}

void leap::graphics::DirectXMesh::Remove()
{
	m_pEngine->RemoveMesh(this);
}

void leap::graphics::DirectXMesh::Release()
{
	if(m_pIndexBuffer) m_pIndexBuffer->Release();
	m_pIndexBuffer = nullptr;
	if(m_pVertexBuffer) m_pVertexBuffer->Release();
	m_pVertexBuffer = nullptr;
	m_NrIndices = 0;
	m_VertexSize = 0;
}