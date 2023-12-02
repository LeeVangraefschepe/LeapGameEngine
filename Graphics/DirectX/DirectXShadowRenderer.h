#pragma once

#include "DirectXRenderTarget.h"

#include <vec2.hpp>
#include <mat4x4.hpp>

#include <vector>
#include <memory>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;

namespace leap::graphics
{
	class DirectXEngine;
	class DirectXMeshRenderer;
	class DirectXMaterial;

	class DirectXShadowRenderer final
	{
	public:
		DirectXShadowRenderer() = default;
		~DirectXShadowRenderer();

		void Create(DirectXEngine* pEngine, const glm::uvec2& shadowMapSize);

		void SetupTarget() const;
		void SetLightMatrix(const glm::mat4x4& lightMatrix) const;
		void Draw(const std::vector<std::unique_ptr<DirectXMeshRenderer>>& pRenderers) const;
		const glm::uvec2& GetShadowMapSize() const { return m_Size; }

		ID3D11ShaderResourceView* GetShadowMap() const;

	private:
		ID3D11DeviceContext* m_pDeviceContext{};

		std::unique_ptr<DirectXMaterial> m_pMaterial{};

		DirectXRenderTarget m_ShadowTarget{};
		glm::uvec2 m_Size{ 3840, 2160 };
	};
}