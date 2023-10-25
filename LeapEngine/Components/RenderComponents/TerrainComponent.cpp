#include "TerrainComponent.h"

#include "MeshRenderer.h"

#include "../../SceneGraph/GameObject.h"
#include "../../ServiceLocator/ServiceLocator.h"

#include <Interfaces/IRenderer.h>

#include <Data/CustomMesh.h>
#include <Shaders/Heightmap.h>

#include <Interfaces/IMaterial.h>
#include <Interfaces/ITexture.h>

leap::TerrainComponent::TerrainComponent()
{
	// Create height information
	m_Heights.resize(static_cast<size_t>(m_Size * m_Size));

	// Create texture
	m_pTexture = ServiceLocator::GetRenderer().CreateTexture(1024, 1024);
}

void leap::TerrainComponent::Awake()
{
	// Create mesh
	MeshRendererComponent* pMeshRenderer{ GetGameObject()->GetComponent<MeshRendererComponent>() };
	pMeshRenderer->LoadMesh("Data/terrain.obj");

	// Create material
	graphics::IMaterial* pMaterial{ ServiceLocator::GetRenderer().CreateMaterial(graphics::shaders::Heightmap::GetShader(), "Heightmap") };
	pMeshRenderer->SetMaterial(pMaterial);

	// Apply texture
	SetTexture(m_pTexture);
}

void leap::TerrainComponent::ApplyHeights()
{
	m_pTexture->SetData(m_Heights.data(), static_cast<unsigned int>(m_Heights.size() * sizeof(float)));
}

void leap::TerrainComponent::SetTexture(leap::graphics::ITexture* pTexture)
{
	m_pTexture = pTexture;
	GetGameObject()->GetComponent<MeshRendererComponent>()->GetMaterial()->SetTexture("gHeightMap", m_pTexture);
}