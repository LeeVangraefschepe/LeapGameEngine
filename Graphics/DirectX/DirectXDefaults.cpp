#include "DirectXDefaults.h"

#include "DirectXMaterial.h"

#include "DirectXMeshLoader.h"

#include "DirectXShaderReader.h"
#include "../Shaders/Pos3D.h"
#include "../Shaders/PosNorm3D.h"

leap::graphics::DirectXDefaults::~DirectXDefaults() = default;

leap::graphics::DirectXMaterial* leap::graphics::DirectXDefaults::GetMaterialNotFound(ID3D11Device* pDevice)
{
	if (m_pNotFoundMaterial == nullptr)
	{
		DirectXShader data{ DirectXShaderReader::GetShaderData(shaders::Pos3D::GetShader()) };
		m_pNotFoundMaterial = std::make_unique<DirectXMaterial>(pDevice, data.path, data.vertexDataFunction);
		m_pNotFoundMaterial->SetFloat4("gColor", glm::vec4{ 1.0f, 0.0f, 1.0f, 1.0f }); // Opaque Pink
	}

	return m_pNotFoundMaterial.get();
}

leap::graphics::DirectXMaterial* leap::graphics::DirectXDefaults::GetMaterialError(ID3D11Device* pDevice)
{
	if (m_pNoMeshMaterial == nullptr)
	{
		DirectXShader data{ DirectXShaderReader::GetShaderData(shaders::PosNorm3D::GetShader()) };
		m_pNoMeshMaterial = std::make_unique<DirectXMaterial>(pDevice, data.path, data.vertexDataFunction);
		m_pNoMeshMaterial->SetFloat4("gColor", glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f }); // Opaque Red
	}

	return m_pNoMeshMaterial.get();
}

void leap::graphics::DirectXDefaults::GetMeshError(ID3D11Device* pDevice, ID3D11Buffer*& pVertexBuffer, ID3D11Buffer*& pIndexBuffer, unsigned int& nrIndices)
{
	const DirectXMeshLoader::DirectXMeshDefinition& mesh{ DirectXMeshLoader::GetInstance().LoadMesh("Data/error.obj", pDevice)};
	pVertexBuffer = mesh.vertexBuffer;
	pIndexBuffer = mesh.indexBuffer;
	nrIndices = mesh.nrIndices;
}

void leap::graphics::DirectXDefaults::Reload(ID3D11Device* pDevice) const
{
	if (m_pNotFoundMaterial) m_pNotFoundMaterial->Reload(pDevice);
	if (m_pNoMeshMaterial) m_pNoMeshMaterial->Reload(pDevice);
}
