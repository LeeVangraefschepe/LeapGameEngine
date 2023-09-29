#pragma once

#include "../Data/RenderData.h"

struct ID3D11Device;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;

namespace leap::graphics
{
	class DirectXRenderTarget final
	{
	public:
		struct RTDesc
		{
			unsigned int width{};
			unsigned int height{};
			bool hasColorTexture{};
			bool hasDepthTexture{};
			bool isColorSRV{};
			bool isDepthSRV{};
			AntiAliasing antiAliasing{ AntiAliasing::NONE };
			ID3D11Texture2D* pOptionalColorBuffer{};
		};

		DirectXRenderTarget() = default;
		~DirectXRenderTarget();

		DirectXRenderTarget(const DirectXRenderTarget& other) = delete;
		DirectXRenderTarget(DirectXRenderTarget&& other) = delete;
		DirectXRenderTarget& operator=(const DirectXRenderTarget& other) = delete;
		DirectXRenderTarget& operator=(DirectXRenderTarget&& other) = delete;

		void Create(ID3D11Device* pDevice, const RTDesc& desc);

		ID3D11DepthStencilView* GetDepthView() { return m_pDepthStencilView; }
		ID3D11RenderTargetView* GetColorView() { return m_pRenderTargetView; }
	private:
		void Release();
		void CreateColor(ID3D11Device* pDevice, const RTDesc& desc);
		void CreateDepth(ID3D11Device* pDevice, const RTDesc& desc);

		ID3D11Texture2D* m_pDepthTexture{ nullptr };
		ID3D11DepthStencilView* m_pDepthStencilView{ nullptr };
		ID3D11Texture2D* m_pRenderTarget{ nullptr };
		ID3D11RenderTargetView* m_pRenderTargetView{ nullptr };
		// add shaderresourceviews (textures basically) to the rendertarget (pDevice->CreateShaderResourceView)
	};
}