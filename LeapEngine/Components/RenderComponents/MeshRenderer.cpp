#include "MeshRenderer.h"

#include "../../ServiceLocator/ServiceLocator.h"

#include <Interfaces/IRenderer.h>
#include <Interfaces/IMaterial.h>

#include <Data/CustomMesh.h>

#include "../Transform/Transform.h"

leap::MeshRendererComponent::MeshRendererComponent()
{
	m_pRenderer = ServiceLocator::GetRenderer().CreateMeshRenderer();
}

void leap::MeshRendererComponent::LoadMesh(const std::string& filePath)
{
	m_pRenderer->LoadMesh(filePath);
}

void leap::MeshRendererComponent::LoadMesh(const graphics::CustomMesh& mesh)
{
	m_pRenderer->LoadMesh(mesh);
}

void leap::MeshRendererComponent::SetMaterial(graphics::IMaterial* pMaterial)
{
	m_pRenderer->SetMaterial(pMaterial);
}

void leap::MeshRendererComponent::LateUpdate()
{
	m_pRenderer->SetTransform(GetTransform()->GetWorldTransform());
}

void leap::MeshRendererComponent::OnDestroy()
{
	ServiceLocator::GetRenderer().RemoveMeshRenderer(m_pRenderer);
}