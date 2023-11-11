#pragma once

#include "../Interfaces/IMeshRenderer.h"

#include "../Data/Vertex.h"

#include "mat4x4.hpp"

#include <vector>
#include <memory>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11InputLayout;
struct ID3D11Buffer;

namespace leap::graphics
{
	class DirectXEngine;
	class IMaterial;
	class DirectXMaterial;

	class DirectXMeshRenderer final : public IMeshRenderer
	{
	public:
		DirectXMeshRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
		virtual ~DirectXMeshRenderer();

		DirectXMeshRenderer(const DirectXMeshRenderer& other) = delete;
		DirectXMeshRenderer(DirectXMeshRenderer&& other) = delete;
		DirectXMeshRenderer& operator=(const DirectXMeshRenderer& other) = delete;
		DirectXMeshRenderer& operator=(DirectXMeshRenderer&& other) = delete;

		virtual void Draw() override;
		virtual void Draw(IMaterial* pMaterial) override;
		virtual IMaterial* GetMaterial() override;
		virtual void SetMaterial(IMaterial* pMaterial) override;
		virtual void SetTransform(const glm::mat4x4& transform) override;
		virtual void LoadMesh(const std::string& filePath) override;
		virtual void LoadMesh(const CustomMesh& mesh) override;
		virtual void SetIsLineRenderer(bool isLineRenderer) override;

		void Reload(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

	private:
		std::string m_FilePath{};

		DirectXMaterial* m_pMaterial{};

		ID3D11Device* m_pDevice{};
		ID3D11DeviceContext* m_pDeviceContext{};

		glm::mat4x4 m_Transform{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

		unsigned int m_VertexSize{};
		ID3D11Buffer* m_pVertexBuffer{};
		unsigned int m_NrIndices{};
		ID3D11Buffer* m_pIndexBuffer{};

		bool m_HasCustomMesh{};
		bool m_IsLineRenderer{};
	};
}