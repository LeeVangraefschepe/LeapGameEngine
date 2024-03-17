#include "DirectXDefaults.h"

#include "DirectXMaterial.h"
#include "DirectXEngine.h"

#include "DirectXShaderReader.h"
#include "../Shaders/Pos3D.h"
#include "../Shaders/PosNorm3D.h"

leap::graphics::DirectXDefaults::~DirectXDefaults() = default;

leap::graphics::DirectXMaterial* leap::graphics::DirectXDefaults::GetMaterialNotFound(DirectXEngine* pEngine)
{
	if (m_pNotFoundMaterial == nullptr)
	{
		DirectXShader data{ DirectXShaderReader::GetShaderData(shaders::Pos3D::GetShader()) };
		m_pNotFoundMaterial = std::make_unique<DirectXMaterial>(pEngine, data.path, data.vertexDataFunction);
		m_pNotFoundMaterial->SetFloat4("gColor", glm::vec4{ 1.0f, 0.0f, 1.0f, 1.0f }); // Opaque Pink
	}

	return m_pNotFoundMaterial.get();
}

leap::graphics::DirectXMaterial* leap::graphics::DirectXDefaults::GetMaterialError(DirectXEngine* pEngine)
{
	if (m_pNoMeshMaterial == nullptr)
	{
		DirectXShader data{ DirectXShaderReader::GetShaderData(shaders::PosNorm3D::GetShader()) };
		m_pNoMeshMaterial = std::make_unique<DirectXMaterial>(pEngine, data.path, data.vertexDataFunction);
		m_pNoMeshMaterial->SetFloat4("gColor", glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f }); // Opaque Red
	}

	return m_pNoMeshMaterial.get();
}

leap::graphics::DirectXMesh* leap::graphics::DirectXDefaults::GetMeshError(DirectXEngine* pEngine)
{
	return static_cast<DirectXMesh*>(pEngine->CreateMesh("Data/Engine/Models/error.obj", false));
}
