#include "DirectXShadowRenderer.h"

#include "DirectXMaterial.h"
#include "DirectXMeshRenderer.h"
#include "DirectXShaderReader.h"
#include "../Shaders/ShadowMap.h"

#include <d3d11.h>

leap::graphics::DirectXShadowRenderer::~DirectXShadowRenderer()
{
}

void leap::graphics::DirectXShadowRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const glm::uvec2& shadowMapSize)
{
	m_pDeviceContext = pDeviceContext;

	DirectXRenderTarget::RTDesc shadowRenderTargetDesc{};
	shadowRenderTargetDesc.width = shadowMapSize.x;
	shadowRenderTargetDesc.height = shadowMapSize.y;
	shadowRenderTargetDesc.hasDepthTexture = true;
	shadowRenderTargetDesc.isDepthSRV = true;
	shadowRenderTargetDesc.antiAliasing = AntiAliasing::NONE;

	m_ShadowTarget.Create(pDevice, pDeviceContext, shadowRenderTargetDesc);

	const DirectXShader shader{ DirectXShaderReader::GetShaderData(shaders::ShadowMap::GetShader()) };
	m_pMaterial = std::make_unique<DirectXMaterial>(pDevice, shader.path, shader.vertexDataFunction);
}

void leap::graphics::DirectXShadowRenderer::SetupTarget() const
{
	m_ShadowTarget.Apply();
	m_ShadowTarget.Clear();
}

void leap::graphics::DirectXShadowRenderer::SetLightMatrix(const glm::mat4x4& lightMatrix) const
{
	m_pMaterial->SetMat4x4("gLightViewProj", lightMatrix);
}

void leap::graphics::DirectXShadowRenderer::Draw(const std::vector<std::unique_ptr<DirectXMeshRenderer>>& pRenderers) const
{
	// Render each mesh
	for (const auto& pRenderer : pRenderers)
	{
		pRenderer->Draw(m_pMaterial.get());
	}
}

ID3D11ShaderResourceView* leap::graphics::DirectXShadowRenderer::GetShadowMap() const
{
	return m_ShadowTarget.GetDepthSRV();
}
