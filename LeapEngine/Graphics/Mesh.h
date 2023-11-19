#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Data/CustomMesh.h>

#include <Debug.h>

namespace leap
{
	class MeshRendererComponent;
	namespace graphics
	{
		class IMesh;
		class CustomMesh;
	}

	class Mesh final
	{
	public:
		Mesh() = default;
		Mesh(const std::string& filePath, bool unique = false);

		template<typename T> 
		void AddVertex(const T& vertex)
		{
			if (!m_pWritableMesh)
			{
				Debug::LogError("LeapEngine Error : Trying to add vertex to a non-writable mesh");
				return;
			}

			// Add the vertex and a new index
			m_pWritableMesh->AddVertex(vertex);
			m_pWritableMesh->AddIndex(static_cast<unsigned int>(m_pWritableMesh->GetIndexBuffer().size()));

			// Set dirty flag
			m_IsWritableMeshDirty = true;
		}

		template<typename T>
		void SetData(const std::vector<T>& vertices, std::vector<unsigned>&& indices = {})
		{
			if (!m_pWritableMesh)
			{
				Debug::LogError("LeapEngine Error : Trying to set vertices to a non-writable mesh");
				return;
			}

			// Clear any previous data in the custom mesh
			m_pWritableMesh->Clear();

			// Set the given indices if any
			const bool hasIndices{ !indices.empty() };
			if (hasIndices) m_pWritableMesh->SetIndices(std::move(indices));

			// Add all the give vertices, also add indices if there were none given by the user
			if (hasIndices)
			{
				for (const T& vertex : vertices) m_pWritableMesh->AddVertex(vertex);
			}
			else
			{
				for (const T& vertex : vertices) AddVertex(vertex);
			}

			// Set dirty flag
			m_IsWritableMeshDirty = true;
		}

		void SetWritable(bool isWritable);
		bool IsWritable() const { return m_pWritableMesh != nullptr; }

		void Load(const std::string& filePath, bool unique = false);

		graphics::IMesh* GetInternal();

	private:
		graphics::IMesh* m_pMesh{};
		std::unique_ptr<graphics::CustomMesh> m_pWritableMesh{};
		bool m_IsWritableMeshDirty{};
	};
}