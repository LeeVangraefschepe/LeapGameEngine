#include "Mesh.h"

#include <Interfaces/IMesh.h>
#include <Interfaces/IRenderer.h>

#include "../ServiceLocator/ServiceLocator.h"

#include <Debug.h>

#include <Vector3.h>

leap::Mesh::Mesh()
	: m_pMesh{ std::make_unique<GraphicsObject<graphics::IMesh>>() }
{
}

leap::Mesh::Mesh(const std::string& filePath, bool unique)
	: Mesh{}
{
	Load(filePath, unique);
}

leap::Mesh::Mesh(graphics::IMesh* pMesh)
	: Mesh{}
{
	m_pMesh->SetObject(pMesh);
}

void leap::Mesh::SetWritable(bool isWritable)
{
	// Don't change the state if the state is already correct
	if (isWritable == IsWritable()) return;

	if (!isWritable)
	{
		// Delete any writable mesh
		m_pWritableMesh = nullptr;
		return;
	}

	// Clear any previously loaded mesh
	m_pMesh->Clear();

	// Create a new writable mesh
	m_pWritableMesh = std::make_unique<WritableMesh>();
}

const std::string& leap::Mesh::GetName() const
{
	if (!m_pMesh)
	{
		static std::string emptyName{};
		return emptyName;
	}

	return m_pMesh->GetUncountedObject()->GetName();
}

std::vector<leap::Mesh> leap::Mesh::GetSubMeshes() const
{
	if (!m_pMesh) return {};

	std::vector<leap::Mesh> subMeshes{};

	const auto& internalSubMeshes{ m_pMesh->GetUncountedObject()->GetSubMeshes() };
	subMeshes.reserve(internalSubMeshes.size());

	for (const auto& pMesh : internalSubMeshes)
	{
		subMeshes.emplace_back(Mesh{ pMesh.get() });
	}

	return subMeshes;
}

bool leap::Mesh::IsValid() const
{
	return m_pMesh && m_pMesh->GetUncountedObject()->IsValid();
}

const glm::vec3& leap::Mesh::GetPosition() const
{
	if (!m_pMesh) return Vector3::Zero();

	return m_pMesh->GetUncountedObject()->GetPosition();
}

const glm::quat& leap::Mesh::GetRotation() const
{
	if (!m_pMesh) return Quaternion::Identity();

	return m_pMesh->GetUncountedObject()->GetRotation();
}

const glm::vec3& leap::Mesh::GetScale() const
{
	if (!m_pMesh) return Vector3::Zero();

	return m_pMesh->GetUncountedObject()->GetScale();
}

void leap::Mesh::Load(const std::string& filePath, bool unique)
{
	// Delete any writable mesh
	m_pWritableMesh = nullptr;

	// Load mesh
	graphics::IMesh* pMesh{ ServiceLocator::GetRenderer().CreateMesh(filePath, !unique) };
	m_pMesh->SetObject(pMesh);
}

leap::graphics::IMesh* leap::Mesh::GetInternal() const
{
	// If mesh is not writable, return stored mesh
	if (!m_pWritableMesh) return m_pMesh ? m_pMesh->GetObject() : nullptr;

	// If mesh is writable but there is no mesh created yet, create an empty mesh
	if (!m_pMesh->IsValid())
	{
		graphics::IMesh* pEmptyMesh{ ServiceLocator::GetRenderer().CreateMesh() };
		m_pMesh->SetObject(pEmptyMesh);
	}

	graphics::IMesh* pMesh{ m_pMesh->GetObject() };

	// Reload the vertices/indices of the mesh if the writable mesh has changed
	if (m_pWritableMesh->isDirty)
	{
		pMesh->ReloadMesh(m_pWritableMesh->mesh);
		m_pWritableMesh->isDirty = false;
	}

	return pMesh;
}
