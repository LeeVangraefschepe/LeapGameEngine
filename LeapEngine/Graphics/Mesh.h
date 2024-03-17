#pragma once

#include "GraphicsObject.h"

#include <Data/CustomMesh.h>
#include <Interfaces/IMesh.h>
#include <Debug.h>

#include <memory>
#include <string>
#include <vector>

#include <vec3.hpp>
#include <Quaternion.h>

namespace leap
{
	namespace graphics
	{
		class CustomMesh;
	}

	class Mesh final
	{
	private:
		struct WritableMesh final
		{
			graphics::CustomMesh mesh{};
			bool isDirty{};
		};

	public:
		Mesh();
		Mesh(const std::string& filePath, bool unique = false);

		template<typename T> 
		void AddVertex(const T& vertex) const
		{
			if (!m_pWritableMesh)
			{
				Debug::LogError("LeapEngine Error : Trying to add vertex to a non-writable mesh");
				return;
			}

			// Add the vertex and a new index
			m_pWritableMesh->mesh.AddVertex(vertex);
			m_pWritableMesh->mesh.AddIndex(static_cast<unsigned int>(m_pWritableMesh->mesh.GetIndexBuffer().size()));

			// Set dirty flag
			m_pWritableMesh->isDirty = true;
		}

		template<typename T>
		void SetData(const std::vector<T>& vertexBuffers, const std::vector<unsigned>& indexBuffers = {}) const
		{
			if (!m_pWritableMesh)
			{
				Debug::LogError("LeapEngine Error : Trying to set vertices to a non-writable mesh");
				return;
			}

			// Clear any previous data in the custom mesh
			m_pWritableMesh->mesh.Clear();

			// Set the given indices if any
			const bool hasIndices{ !indexBuffers.empty() };
			if (hasIndices) m_pWritableMesh->mesh.SetIndices(indexBuffers);

			// Add all the give vertices, also add indices if there were none given by the user
			if (hasIndices)
			{
				for (const T& vertex : vertexBuffers) m_pWritableMesh->mesh.AddVertex(vertex);
			}
			else
			{
				for (const T& vertex : vertexBuffers) AddVertex(vertex);
			}

			// Set dirty flag
			m_pWritableMesh->isDirty = true;
		}

		void SetWritable(bool isWritable);
		bool IsWritable() const { return m_pWritableMesh != nullptr; }

		const std::string& GetName() const;
		std::vector<Mesh> GetSubMeshes() const;
		bool IsValid() const;
		const glm::vec3& GetPosition() const;
		const glm::quat& GetRotation() const;
		const glm::vec3& GetScale() const;

		void Load(const std::string& filePath, bool unique = false);

		graphics::IMesh* GetInternal() const;

	private:
		Mesh(graphics::IMesh* pMesh);

		std::unique_ptr<GraphicsObject<graphics::IMesh>> m_pMesh{};
		std::unique_ptr<WritableMesh> m_pWritableMesh{};
	};
}