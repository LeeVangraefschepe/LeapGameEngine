#include "MeshRenderer.h"

#include "../../ServiceLocator/ServiceLocator.h"

#include <Interfaces/IRenderer.h>
#include <Interfaces/IMaterial.h>

leap::MeshRendererComponent::MeshRendererComponent()
{
	m_pRenderer = ServiceLocator::GetRenderer().CreateMeshRenderer();
}

void leap::MeshRendererComponent::SetMaterial(graphics::IMaterial* pMaterial)
{
	m_pRenderer->SetMaterial(pMaterial);
}