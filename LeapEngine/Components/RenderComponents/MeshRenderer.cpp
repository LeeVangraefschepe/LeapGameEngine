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

leap::graphics::IMaterial* leap::MeshRendererComponent::GetMaterial() const
{
	return m_pRenderer->GetMaterial();
}

void leap::MeshRendererComponent::Awake()
{
	GetTransform()->OnPositionChanged.AddListener(this);
	GetTransform()->OnRotationChanged.AddListener(this);
	GetTransform()->OnScaleChanged.AddListener(this);
}

void leap::MeshRendererComponent::LateUpdate()
{
	if (m_IsDirty)
	{
		m_pRenderer->SetTransform(GetTransform()->GetWorldTransform());
		m_IsDirty = false;
	}
}

void leap::MeshRendererComponent::OnDestroy()
{
	GetTransform()->OnPositionChanged.RemoveListener(this);
	GetTransform()->OnRotationChanged.RemoveListener(this);
	GetTransform()->OnScaleChanged.RemoveListener(this);
	ServiceLocator::GetRenderer().RemoveMeshRenderer(m_pRenderer);
}

void leap::MeshRendererComponent::Notify()
{
	m_IsDirty = true;
}
