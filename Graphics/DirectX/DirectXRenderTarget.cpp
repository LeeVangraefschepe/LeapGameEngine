#include "DirectXRenderTarget.h"

#include <d3d11.h>
#include <stdexcept>

leap::graphics::DirectXRenderTarget::~DirectXRenderTarget()
{
	Release();
}

void leap::graphics::DirectXRenderTarget::Create(ID3D11Device* pDevice, const RTDesc& desc)
{
	Release();
	CreateDepth(pDevice, desc);
	CreateColor(pDevice, desc);
}

void leap::graphics::DirectXRenderTarget::CreateDepth(ID3D11Device* pDevice, const RTDesc& desc)
{
	if (desc.hasDepthTexture)
	{
		D3D11_TEXTURE2D_DESC depthStencilDesc{};
		depthStencilDesc.Width = desc.width;
		depthStencilDesc.Height = desc.height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = static_cast<UINT>(desc.antiAliasing);
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
		depthStencilViewDesc.Format = depthStencilDesc.Format;
		switch (desc.antiAliasing)
		{
		case AntiAliasing::NONE:
		{
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			break;
		}
		default:
		{
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			break;
		}
		}
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		HRESULT result{ pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthTexture) };
		if (FAILED(result)) throw std::runtime_error{ "Failed to create depth texture for a render target" };

		result = pDevice->CreateDepthStencilView(m_pDepthTexture, &depthStencilViewDesc, &m_pDepthStencilView);
		if (FAILED(result)) throw std::runtime_error{ "Failed to create depth stencil view for a render target" };
	}
}

void leap::graphics::DirectXRenderTarget::Release()
{
	if (m_pDepthTexture) m_pDepthTexture->Release();
	if (m_pDepthStencilView) m_pDepthStencilView->Release();
	if (m_pRenderTarget) m_pRenderTarget->Release();
	if (m_pRenderTargetView) m_pRenderTargetView->Release();
}

void leap::graphics::DirectXRenderTarget::CreateColor(ID3D11Device* pDevice, const RTDesc& desc)
{
	if (desc.hasColorTexture)
	{
		if (desc.pOptionalColorBuffer)
		{
			m_pRenderTarget = desc.pOptionalColorBuffer;
		}
		else
		{
			D3D11_TEXTURE2D_DESC colorTextureDesc{};
			colorTextureDesc.Width = desc.width;
			colorTextureDesc.Height = desc.height;
			colorTextureDesc.MipLevels = 1;
			colorTextureDesc.ArraySize = 1;
			colorTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			colorTextureDesc.SampleDesc.Count = static_cast<UINT>(desc.antiAliasing);
			colorTextureDesc.SampleDesc.Quality = 0;
			colorTextureDesc.Usage = D3D11_USAGE_DEFAULT;
			colorTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
			colorTextureDesc.CPUAccessFlags = 0;
			colorTextureDesc.MiscFlags = 0;

			const HRESULT result{ pDevice->CreateTexture2D(&colorTextureDesc, nullptr, &m_pRenderTarget) };
			if (FAILED(result)) throw std::runtime_error{ "Failed to create color texture for a render target" };
		}

		HRESULT result{ pDevice->CreateRenderTargetView(m_pRenderTarget, nullptr, &m_pRenderTargetView) };
		if (FAILED(result)) throw std::runtime_error{ "Failed to create render target view for a render target" };
	}
}