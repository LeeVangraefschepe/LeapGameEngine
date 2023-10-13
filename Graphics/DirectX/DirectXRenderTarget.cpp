#include "DirectXRenderTarget.h"

#include <d3d11.h>
#include "Debug.h"

leap::graphics::DirectXRenderTarget::~DirectXRenderTarget()
{
	Release();
}

void leap::graphics::DirectXRenderTarget::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const RTDesc& desc)
{
	m_pDeviceContext = pDeviceContext;

	m_Size = { desc.width, desc.height };

	Release();
	CreateDepth(pDevice, desc);
	CreateColor(pDevice, desc);
}

void leap::graphics::DirectXRenderTarget::Apply() const
{
	// Set render targets
	ID3D11RenderTargetView* pRenderTargetView{ m_pRenderTargetView };
	m_pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, m_pDepthStencilView);

	// Set viewport
	D3D11_VIEWPORT viewport{};
	viewport.Width = static_cast<FLOAT>(m_Size.x);
	viewport.Height = static_cast<FLOAT>(m_Size.y);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	m_pDeviceContext->RSSetViewports(1, &viewport);
}

void leap::graphics::DirectXRenderTarget::Clear(const glm::vec4& clearColor) const
{
	if(m_pRenderTargetView)
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, &clearColor.r);

	if (m_pDepthStencilView)
		m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void leap::graphics::DirectXRenderTarget::Clear() const
{
	if (m_pRenderTargetView)
	{
		constexpr glm::vec4 clearColor{ 0.0f, 0.0f, 0.0f, 1.0f };
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, &clearColor.r);
	}

	if (m_pDepthStencilView)
		m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void leap::graphics::DirectXRenderTarget::CreateDepth(ID3D11Device* pDevice, const RTDesc& desc)
{
	if (!desc.hasDepthTexture) return;

	D3D11_TEXTURE2D_DESC depthStencilDesc{};
	depthStencilDesc.Width = desc.width;
	depthStencilDesc.Height = desc.height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = GetDepthResourceFormat(desc.depthFormat);
	depthStencilDesc.SampleDesc.Count = static_cast<UINT>(desc.antiAliasing);
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | (desc.isDepthSRV ? D3D11_BIND_SHADER_RESOURCE : 0);
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = desc.depthFormat;
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
	if (FAILED(result)) Debug::LogError("DirectXEngine Error: Failed to create depth texture for a render target");

	result = pDevice->CreateDepthStencilView(m_pDepthTexture, &depthStencilViewDesc, &m_pDepthStencilView);
	if (FAILED(result)) Debug::LogError("DirectXEngine Error: Failed to create depth stencil view for a render target");

	if (desc.isDepthSRV)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC depthSrvDesc;
		ZeroMemory(&depthSrvDesc, sizeof(depthSrvDesc));

		depthSrvDesc.Format = GetDepthShaderResourceViewFormat(desc.depthFormat);
		depthSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		depthSrvDesc.Texture2D.MipLevels = 1;
		depthSrvDesc.Texture2D.MostDetailedMip = 0;

		result = pDevice->CreateShaderResourceView(m_pDepthTexture, &depthSrvDesc, &m_pDepthShaderResourceView);
		if (FAILED(result)) Debug::LogError("DirectXEngine Error: Failed to create depth shader resource view for a render target");
	}
}

DXGI_FORMAT leap::graphics::DirectXRenderTarget::GetDepthResourceFormat(DXGI_FORMAT srcFormat)
{
	switch (srcFormat)
	{
	case DXGI_FORMAT::DXGI_FORMAT_D16_UNORM:
		return DXGI_FORMAT::DXGI_FORMAT_R16_TYPELESS;
	case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
		return DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
	case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT:
		return DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
	case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		return DXGI_FORMAT::DXGI_FORMAT_R32G8X24_TYPELESS;
	default:
		Debug::LogError("DirectXEngine Error: This DXGI format is not supported for depth");
		return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	}
}

DXGI_FORMAT leap::graphics::DirectXRenderTarget::GetDepthShaderResourceViewFormat(DXGI_FORMAT srcFormat)
{
	switch (srcFormat)
	{
	case DXGI_FORMAT::DXGI_FORMAT_D16_UNORM:
		return DXGI_FORMAT::DXGI_FORMAT_R16_FLOAT;
	case DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT:
		return DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT:
		return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	case DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
	default:
		Debug::LogError("DirectXEngine Error: This DXGI format is not supported for depth SRV");
		return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	}
}

void leap::graphics::DirectXRenderTarget::Release()
{
	if (m_pDepthTexture) m_pDepthTexture->Release();
	if (m_pDepthStencilView) m_pDepthStencilView->Release();
	if (m_pDepthShaderResourceView) m_pDepthShaderResourceView->Release();
	if (m_pRenderTarget) m_pRenderTarget->Release();
	if (m_pRenderTargetView) m_pRenderTargetView->Release();
	if (m_pRenderTargetShaderResourceView) m_pRenderTargetShaderResourceView->Release();
}

void leap::graphics::DirectXRenderTarget::CreateColor(ID3D11Device* pDevice, const RTDesc& desc)
{
	if (!desc.hasColorTexture) return;

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
		colorTextureDesc.Format = desc.colorFormat;
		colorTextureDesc.SampleDesc.Count = static_cast<UINT>(desc.antiAliasing);
		colorTextureDesc.SampleDesc.Quality = 0;
		colorTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		colorTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | (desc.isColorSRV ? D3D11_BIND_SHADER_RESOURCE : 0);
		colorTextureDesc.CPUAccessFlags = 0;
		colorTextureDesc.MiscFlags = 0;

		const HRESULT result{ pDevice->CreateTexture2D(&colorTextureDesc, nullptr, &m_pRenderTarget) };
		if (FAILED(result)) Debug::LogError("DirectXEngine Error: Failed to create color texture for a render target");
	}

	HRESULT result{ pDevice->CreateRenderTargetView(m_pRenderTarget, nullptr, &m_pRenderTargetView) };
	if (FAILED(result)) Debug::LogError("DirectXEngine Error: Failed to create render target view for a render target");

	if (desc.isColorSRV)
	{
		result = pDevice->CreateShaderResourceView(m_pRenderTarget, nullptr, &m_pRenderTargetShaderResourceView);
		if (FAILED(result)) Debug::LogError("DirectXEngine Error: Failed to create color shader resource view for a render target");
	}
}