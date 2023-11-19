#include "MeshRenderer.h"

#include "../../ServiceLocator/ServiceLocator.h"

#include <Interfaces/IRenderer.h>
#include <Interfaces/IMaterial.h>

#include <Data/CustomMesh.h>

#include "../Transform/Transform.h"
#include "../../Graphics/Mesh.h"

leap::MeshRenderer::MeshRenderer()
{
	m_pRenderer = ServiceLocator::GetRenderer().CreateMeshRenderer();
}

void leap::MeshRenderer::SetMesh(Mesh& mesh)
{
	m_pRenderer->SetMesh(mesh.GetInternal());
}

void leap::MeshRenderer::SetMaterial(graphics::IMaterial* pMaterial)
{
	m_pRenderer->SetMaterial(pMaterial);
}

leap::graphics::IMaterial* leap::MeshRenderer::GetMaterial() const
{
	return m_pRenderer->GetMaterial();
}

void leap::MeshRenderer::Awake()
{
	GetTransform()->OnPositionChanged.AddListener(this);
	GetTransform()->OnRotationChanged.AddListener(this);
	GetTransform()->OnScaleChanged.AddListener(this);
}

void leap::MeshRenderer::LateUpdate()
{
	if (m_IsDirty)
	{
		m_pRenderer->SetTransform(GetTransform()->GetWorldTransform());
		m_IsDirty = false;
	}
}

void leap::MeshRenderer::OnDestroy()
{
	GetTransform()->OnPositionChanged.RemoveListener(this);
	GetTransform()->OnRotationChanged.RemoveListener(this);
	GetTransform()->OnScaleChanged.RemoveListener(this);
	ServiceLocator::GetRenderer().RemoveMeshRenderer(m_pRenderer);
}

void leap::MeshRenderer::Notify()
{
	m_IsDirty = true;
}
