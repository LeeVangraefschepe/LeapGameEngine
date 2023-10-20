#pragma once

#include "../Data/RenderData.h"

#include <vec2.hpp>
#include <vec4.hpp>

#include <dxgiformat.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;

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
			DXGI_FORMAT colorFormat{ DXGI_FORMAT_R8G8B8A8_UNORM };
			DXGI_FORMAT depthFormat{ DXGI_FORMAT_D32_FLOAT };
			AntiAliasing antiAliasing{ AntiAliasing::NONE };
			ID3D11Texture2D* pOptionalColorBuffer{};
		};

		DirectXRenderTarget() = default;
		~DirectXRenderTarget();

		DirectXRenderTarget(const DirectXRenderTarget& other) = delete;
		DirectXRenderTarget(DirectXRenderTarget&& other) = delete;
		DirectXRenderTarget& operator=(const DirectXRenderTarget& other) = delete;
		DirectXRenderTarget& operator=(DirectXRenderTarget&& other) = delete;

		void Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const RTDesc& desc);

		void Apply() const;
		void Clear(const glm::vec4& clearColor) const;
		void Clear() const;

		ID3D11ShaderResourceView* GetDepthSRV() const { return m_pDepthShaderResourceView; }
		ID3D11ShaderResourceView* GetColorSRV() const { return m_pRenderTargetShaderResourceView; }
		const glm::uvec2& GetSize() const { return m_Size; }
	private:
		void Release();
		void CreateColor(ID3D11Device* pDevice, const RTDesc& desc);
		void CreateDepth(ID3D11Device* pDevice, const RTDesc& desc);

		DXGI_FORMAT GetDepthResourceFormat(DXGI_FORMAT srcFormat);
		DXGI_FORMAT GetDepthShaderResourceViewFormat(DXGI_FORMAT srcFormat);

		ID3D11DeviceContext* m_pDeviceContext{};
		glm::uvec2 m_Size{};

		ID3D11Texture2D* m_pDepthTexture{};
		ID3D11DepthStencilView* m_pDepthStencilView{};
		ID3D11Texture2D* m_pRenderTarget{};
		ID3D11RenderTargetView* m_pRenderTargetView{};
		ID3D11ShaderResourceView* m_pDepthShaderResourceView{};
		ID3D11ShaderResourceView* m_pRenderTargetShaderResourceView{};
	};
}