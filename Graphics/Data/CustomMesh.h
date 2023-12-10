#pragma once

#include <Debug.h>

#include <vector>

namespace leap::graphics
{
	class CustomMesh final
	{
	public:
		template<typename T>
		void AddVertex(const T& vertex)
		{
			constexpr unsigned int vertexSize{ sizeof(T) };

			if (m_PrevSize != 0 && vertexSize != m_PrevSize) Debug::LogError("LeapEngine Graphics Error : Custom Mesh received two different kinds of vertex types");

			const size_t vertexStart{ m_Vertices.size() };

			m_Vertices.resize(m_Vertices.size() + vertexSize);
			memcpy_s(&m_Vertices[vertexStart], vertexSize, &vertex, vertexSize);

			m_PrevSize = vertexSize;
		}
		void AddIndex(unsigned int index)
		{
			m_Indices.emplace_back(index);
		}
		void SetIndices(const std::vector<unsigned int>& indices)
		{
			m_Indices = indices;
		}

		/// <summary>
		/// Removes all existing vertices & indices, but keeps the capacity of the internal vectors
		/// </summary>
		void Clear()
		{
			m_Vertices.clear();
			m_Indices.clear();
			m_PrevSize = 0;
		}

		const std::vector<unsigned char>& GetVertexBuffer() const { return m_Vertices; }
		unsigned int GetVertexSize() const { return m_PrevSize; }
		const std::vector<unsigned int>& GetIndexBuffer() const { return m_Indices; }

	private:
		unsigned int m_PrevSize{};

		std::vector<unsigned char> m_Vertices{};
		std::vector<unsigned int> m_Indices{};
	};
}