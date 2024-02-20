#include "MeshRenderer.h"

#include "../../ServiceLocator/ServiceLocator.h"

#include <Interfaces/IRenderer.h>
#include <Interfaces/IMaterial.h>

#include <Data/CustomMesh.h>

#include "../Transform/Transform.h"
#include "../../Graphics/Mesh.h"
#include "../../Graphics/Material.h"

leap::MeshRenderer::MeshRenderer()
{
	m_pRenderer = ServiceLocator::GetRenderer().CreateMeshRenderer();
}

void leap::MeshRenderer::SetMesh(const Mesh& mesh)
{
	m_pRenderer->SetMesh(mesh.GetInternal());
}

void leap::MeshRenderer::SetMaterial(const Material& material)
{
	m_pRenderer->SetMaterial(material.GetInternal());
}

leap::graphics::IMaterial* leap::MeshRenderer::GetMaterial() const
{
	return m_pRenderer->GetMaterial();
}

void leap::MeshRenderer::Awake()
{
	GetTransform()->OnPositionChangedDelegate.Bind(this, &MeshRenderer::OnTransformChanged);
	GetTransform()->OnRotationChangedDelegate.Bind(this, &MeshRenderer::OnTransformChanged);
	GetTransform()->OnScaleChangedDelegate.Bind(this, &MeshRenderer::OnTransformChanged);
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
	GetTransform()->OnPositionChangedDelegate.Unbind(this);
	GetTransform()->OnRotationChangedDelegate.Unbind(this);
	GetTransform()->OnScaleChangedDelegate.Unbind(this);
	ServiceLocator::GetRenderer().RemoveMeshRenderer(m_pRenderer);
}

void leap::MeshRenderer::OnTransformChanged()
{
	m_IsDirty = true;
}
