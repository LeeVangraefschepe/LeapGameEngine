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
	MeshData meshData{};

	if (filePath.length() > 4 && filePath.substr(filePath.length() - 4, 4) == ".obj")
	{
		// Load obj
		std::vector<Vertex> vertexBuffers{};
		std::vector<unsigned int> indexBuffers{};

		MeshLoader::ParseObj(filePath, vertexBuffers, indexBuffers);

		meshData.vertexBuffers.emplace_back(std::move(vertexBuffers));
		meshData.indexBuffers.emplace_back(std::move(indexBuffers));
	}
	else
	{
		// Load fbx
		MeshLoader::ParseFbx(filePath, meshData);
	}

	if (meshData.vertexBuffers.size() != meshData.indexBuffers.size())
	{
		leap::Debug::LogError("Mesh loader generated different amounts of vertex and index buffers.");
		return;
	}

	ApplyMeshData(pEngine, meshData);
}

leap::graphics::DirectXMesh::DirectXMesh(DirectXEngine* pEngine, const MeshData& meshData)
	: DirectXMesh(pEngine)
{
	ApplyMeshData(pEngine, meshData);
}

void leap::graphics::DirectXMesh::ApplyMeshData(DirectXEngine* pEngine, const MeshData& meshData)
{
	for (size_t i = 0; i < meshData.vertexBuffers.size(); ++i)
	{
		const std::vector<Vertex>& vertexBuffers{ meshData.vertexBuffers[i] };
		const std::vector<unsigned int>& indexBuffers{ meshData.indexBuffers[i] };

		DirectXBuffers buffers{};

		// Set index amoutn and vertex size
		buffers.m_NrIndices = static_cast<unsigned int>(indexBuffers.size());
		buffers.m_VertexSize = sizeof(Vertex);

		// Create vertex buffer
		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(Vertex) * static_cast<unsigned int>(vertexBuffers.size());
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem = vertexBuffers.data();

		HRESULT result{ pEngine->GetDevice()->CreateBuffer(&bd, &initData, &buffers.m_pVertexBuffer) };
		if (FAILED(result))
		{
			Debug::LogError("DirectXEngine Error : Failed to create vertex buffer from obj");
			return;
		}

		// Create index buffer
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(unsigned int) * static_cast<unsigned int>(indexBuffers.size());
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		initData.pSysMem = indexBuffers.data();

		result = pEngine->GetDevice()->CreateBuffer(&bd, &initData, &buffers.m_pIndexBuffer);
		if (FAILED(result)) Debug::LogError("DirectXEngine Error : Failed to create index buffer from obj");

		m_Buffers.emplace_back(buffers);
	}

	m_Position = meshData.position;
	m_Rotation = meshData.rotation;
	m_Scale = meshData.scale;

	for (const auto& subMesh : meshData.subMeshes)
	{
		m_SubMeshes.emplace_back(new DirectXMesh{ pEngine, subMesh });
	}
}

leap::graphics::DirectXMesh::~DirectXMesh()
{
	Release();
}

const std::vector<std::unique_ptr<leap::graphics::IMesh>>& leap::graphics::DirectXMesh::GetSubMeshes()
{
	return m_SubMeshes;
}

const std::string& leap::graphics::DirectXMesh::GetName()
{
	return m_Name;
}

void leap::graphics::DirectXMesh::ReloadMesh(const CustomMesh& mesh)
{
	Release();

	// Keep the mesh empty if there is no vertex buffer given in the mesh
	if (mesh.GetVertexBuffer().empty()) return;

	m_Buffers.clear();

	DirectXBuffers buffers{};

	// Set index amoutn and vertex size
	buffers.m_NrIndices = static_cast<unsigned int>(mesh.GetIndexBuffer().size());
	buffers.m_VertexSize = mesh.GetVertexSize();
	
	// Create vertex buffer
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = static_cast<unsigned>(mesh.GetVertexBuffer().size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = mesh.GetVertexBuffer().data();

	HRESULT result{ m_pEngine->GetDevice()->CreateBuffer(&bd, &initData, &buffers.m_pVertexBuffer)};
	if (FAILED(result))
	{
		Debug::LogError("DirectXEngine Error : Failed to create vertex buffer from obj");
		return;
	}

	// Create index buffer
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(unsigned int) * static_cast<unsigned int>(buffers.m_NrIndices);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = mesh.GetIndexBuffer().data();

	result = m_pEngine->GetDevice()->CreateBuffer(&bd, &initData, &buffers.m_pIndexBuffer);
	if (FAILED(result)) Debug::LogError("DirectXEngine Error : Failed to create index buffer from obj");

	m_Buffers.emplace_back(buffers);
}

void leap::graphics::DirectXMesh::Remove()
{
	m_pEngine->RemoveMesh(this);
}

void leap::graphics::DirectXMesh::Release()
{
	for (auto& buffer : m_Buffers)
	{
		if (buffer.m_pIndexBuffer) buffer.m_pIndexBuffer->Release();
		if (buffer.m_pVertexBuffer) buffer.m_pVertexBuffer->Release();
	}
	m_Buffers.clear();
}

bool leap::graphics::DirectXMesh::IsValid()
{
	return !m_Buffers.empty();
}

const glm::vec3& leap::graphics::DirectXMesh::GetPosition()
{
	return m_Position;
}

const glm::quat& leap::graphics::DirectXMesh::GetRotation()
{
	return m_Rotation;
}

const glm::vec3& leap::graphics::DirectXMesh::GetScale()
{
	return m_Scale;
}