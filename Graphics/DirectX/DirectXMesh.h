#pragma once

#include "../Interfaces/IMesh.h"
#include "../Data/MeshData.h"

#include <string>
#include <vector>

#include <Quaternion.h>
#include <Vector3.h>

struct ID3D11Buffer;

namespace leap::graphics
{
	class DirectXEngine;
	class DirectXMeshRenderer;

	class DirectXMesh final : public IMesh
	{
	public:
		struct DirectXBuffers
		{
			unsigned int m_VertexSize{};
			ID3D11Buffer* m_pVertexBuffer{};
			unsigned int m_NrIndices{};
			ID3D11Buffer* m_pIndexBuffer{};
		};

		DirectXMesh(DirectXEngine* pEngine);
		DirectXMesh(DirectXEngine* pEngine, const std::string& filePath);

		virtual ~DirectXMesh();

		DirectXMesh(const DirectXMesh& other) = delete;
		DirectXMesh(DirectXMesh&& other) = delete;
		DirectXMesh& operator=(const DirectXMesh& other) = delete;
		DirectXMesh& operator=(DirectXMesh&& other) = delete;

		virtual const std::vector<std::unique_ptr<IMesh>>& GetSubMeshes() override;
		virtual const std::string& GetName() override;
		virtual void ReloadMesh(const CustomMesh& mesh) override;
		virtual void Remove() override;
		virtual bool IsValid() override;
		virtual const glm::vec3& GetPosition() override;
		virtual const glm::quat& GetRotation() override;
		virtual const glm::vec3& GetScale() override;

	private:
		DirectXMesh(DirectXEngine* pEngine, const MeshData& meshData);

		void ApplyMeshData(DirectXEngine* pEngine, const MeshData& meshData);

		void Release();

		friend DirectXMeshRenderer;

		// List of buffers so there can be multiple materials per mesh
		std::vector<DirectXBuffers> m_Buffers{};

		std::string m_Name{};
		std::vector<std::unique_ptr<IMesh>> m_SubMeshes{};

		glm::vec3 m_Position{};
		glm::quat m_Rotation{ Quaternion::Identity() };
		glm::vec3 m_Scale{ Vector3::One() };

		DirectXEngine* m_pEngine{};
	};
}