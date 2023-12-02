#pragma once

#include "../Interfaces/IMeshRenderer.h"

#include "../Data/Vertex.h"

#include "mat4x4.hpp"
#include <Matrix.h>

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
	class DirectXMesh;

	class DirectXMeshRenderer final : public IMeshRenderer
	{
	public:
		DirectXMeshRenderer(DirectXEngine* pEngine);
		virtual ~DirectXMeshRenderer();

		DirectXMeshRenderer(const DirectXMeshRenderer& other) = delete;
		DirectXMeshRenderer(DirectXMeshRenderer&& other) = delete;
		DirectXMeshRenderer& operator=(const DirectXMeshRenderer& other) = delete;
		DirectXMeshRenderer& operator=(DirectXMeshRenderer&& other) = delete;

		virtual void Draw() override;
		virtual void Draw(IMaterial* pMaterial) override;
		virtual IMaterial* GetMaterial() override;
		virtual void SetMaterial(IMaterial* pMaterial) override;
		virtual void UnsetMaterial() override;
		virtual void SetTransform(const glm::mat4x4& transform) override;
		virtual void SetMesh(IMesh* pMesh) override;
		virtual void UnsetMesh() override;
		virtual void SetIsLineRenderer(bool isLineRenderer) override;

	private:
		DirectXMesh* m_pMesh{};
		DirectXMaterial* m_pMaterial{};

		DirectXEngine* m_pEngine{};

		glm::mat4x4 m_Transform{ Matrix::Identity4x4() };

		bool m_IsLineRenderer{};
	};
}