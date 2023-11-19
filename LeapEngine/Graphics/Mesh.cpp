#include "Mesh.h"

#include <Interfaces/IMesh.h>
#include <Interfaces/IRenderer.h>

#include "../ServiceLocator/ServiceLocator.h"

#include <Debug.h>

leap::Mesh::Mesh(const std::string& filePath, bool unique)
{
	m_pMesh = ServiceLocator::GetRenderer().CreateMesh(filePath, !unique);
}

void leap::Mesh::SetWritable(bool isWritable)
{
	// Don't change the state if the state is already correct
	if (isWritable == IsWritable()) return;

	if (isWritable)
	{
		if (m_pMesh)
		{
			Debug::LogWarning("LeapEngine Warning : Meshes that are read from files cannot be made writable");
			return;
		}

		m_pWritableMesh = std::make_unique<graphics::CustomMesh>();
		m_IsWritableMeshDirty = true;
		return;
	}

	m_pWritableMesh = nullptr;
	m_IsWritableMeshDirty = false;
}

void leap::Mesh::Load(const std::string& filePath, bool unique)
{
	// Delete any writable mesh
	m_pWritableMesh = nullptr;
	m_IsWritableMeshDirty = false;

	// Load mesh
	m_pMesh = ServiceLocator::GetRenderer().CreateMesh(filePath, !unique);
}

leap::graphics::IMesh* leap::Mesh::GetInternal()
{
	if (!m_pWritableMesh) return m_pMesh;

	if (!m_pMesh) m_pMesh = ServiceLocator::GetRenderer().CreateMesh();

	if (m_IsWritableMeshDirty)
	{
		m_pMesh->ReloadMesh(*m_pWritableMesh);
		m_IsWritableMeshDirty = false;
	}

	return m_pMesh;
}
