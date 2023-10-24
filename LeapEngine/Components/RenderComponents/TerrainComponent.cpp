#include "TerrainComponent.h"

#include "MeshRenderer.h"

#include "../../SceneGraph/GameObject.h"
#include "../../ServiceLocator/ServiceLocator.h"

#include <Interfaces/IRenderer.h>

#include <Data/CustomMesh.h>
#include <Shaders/Heightmap.h>

#include <Interfaces/IMaterial.h>
#include <Interfaces/ITexture.h>

void leap::TerrainComponent::Awake()
{
	MeshRendererComponent* pMeshRenderer{ GetGameObject()->GetComponent<MeshRendererComponent>() };

	/*m_pTexture = ServiceLocator::GetRenderer().CreateTexture(1024, 1024);
	for (int x{}; x < 512; ++x)
	{
		for (int y{}; y < 1024; ++y)
		{
			m_pTexture->SetPixel(x, y, glm::vec4(1, 1, 1, 1));
		}
	}*/
	m_pTexture = ServiceLocator::GetRenderer().CreateTexture("Data/Heightmap.png");

	pMeshRenderer->LoadMesh("Data/terrain.obj");

	graphics::IMaterial* pMaterial{ ServiceLocator::GetRenderer().CreateMaterial(graphics::shaders::Heightmap::GetShader(), "Heightmap") };
	pMaterial->SetTexture("gHeightMap", m_pTexture);
	
	pMeshRenderer->SetMaterial(pMaterial);
}
