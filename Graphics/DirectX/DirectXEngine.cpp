#include "glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw3native.h"
#include "DirectXEngine.h"
#include "DirectXTex.h"

#include <glm.hpp>

#include "../Camera.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>

#include <stdexcept>
#include <iostream>

#include "DirectXShaderReader.h"
#include "DirectXTexture.h"
#include "DirectXMeshLoader.h"
#include "DirectXMaterial.h"

#include "../Shaders/PosNormTex3D.h"

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "imgui_impl_dx11.h"

leap::graphics::DirectXEngine::DirectXEngine(GLFWwindow* pWindow) : m_pWindow(pWindow)
{
	std::cout << "DirectXRenderer Log: Created DirectX engine\n";
}

leap::graphics::DirectXEngine::~DirectXEngine()
{
	Release();
	std::cout << "DirectXRenderer Log: Destroyed DirectX engine\n";
}

void leap::graphics::DirectXEngine::Initialize()
{
	ReloadDirectXEngine();

	std::cout << "DirectXRenderer Log: Creating default material with ID \"Default\"\n";
	CreateMaterial(shaders::PosNormTex3D::GetShader(), "Default");

	m_IsInitialized = true;
	std::cout << "DirectXRenderer Log: Successfully initialized DirectX engine\n";
}

void leap::graphics::DirectXEngine::SetAntiAliasing(AntiAliasing antiAliasing)
{
	m_AntiAliasing = antiAliasing;

	if(m_IsInitialized) ReloadDirectXEngine();
}

void leap::graphics::DirectXEngine::SetWindowSize(const glm::ivec2&)
{
	if (m_IsInitialized) ReloadDirectXEngine();
}

void leap::graphics::DirectXEngine::SetShadowMapData(unsigned int shadowMapWidth, unsigned int shadowMapHeight, float orthoSize, float nearPlane, float farPlane)
{
	m_DirectionalLight.SetShadowMapData(static_cast<float>(shadowMapWidth) / shadowMapHeight, orthoSize, nearPlane, farPlane);
	m_ShadowRenderer.Create(m_pDevice, m_pDeviceContext, { shadowMapWidth, shadowMapHeight });
}

leap::graphics::IMeshRenderer* leap::graphics::DirectXEngine::CreateMeshRenderer()
{
	m_pRenderers.push_back(std::make_unique<DirectXMeshRenderer>(m_pDevice, m_pDeviceContext));
	return m_pRenderers[m_pRenderers.size() - 1].get();
}

void leap::graphics::DirectXEngine::RemoveMeshRenderer(IMeshRenderer* pMeshRenderer)
{
	m_pRenderers.erase(std::remove_if(begin(m_pRenderers), end(m_pRenderers), [pMeshRenderer](const auto& pRenderer) { return pMeshRenderer == pRenderer.get(); }));
}

leap::graphics::IMaterial* leap::graphics::DirectXEngine::CreateMaterial(std::unique_ptr<Shader, ShaderDelete> pShader, const std::string& name)
{
	if (auto it{ m_pMaterials.find(name) }; it != end(m_pMaterials))
	{
		return it->second.get();
	}

	const DirectXShader shader{ DirectXShaderReader::GetShaderData(std::move(pShader)) };
	auto pMaterial{ std::make_unique<DirectXMaterial>(m_pDevice, shader.path, shader.vertexDataFunction) };
	auto pMaterialRaw{ pMaterial.get() };

	pMaterial->SetTexture("gShadowMap", m_ShadowRenderer.GetShadowMap());

	m_pMaterials[name] = std::move(pMaterial);
	return pMaterialRaw;
}

leap::graphics::IMaterial* leap::graphics::DirectXEngine::CloneMaterial(const std::string& original, const std::string& clone)
{
	if (auto it{ m_pMaterials.find(clone) }; it != end(m_pMaterials))
	{
		return it->second.get();
	}

	if (auto it{ m_pMaterials.find(original) }; it != end(m_pMaterials))
	{
		std::unique_ptr<DirectXMaterial> pMaterial{ it->second->Clone(m_pDevice) };
		auto pMaterialRaw{ pMaterial.get() };

		pMaterial->SetTexture("gShadowMap", m_ShadowRenderer.GetShadowMap());

		m_pMaterials[clone] = std::move(pMaterial);
		return pMaterialRaw;
	}

	return nullptr;
}

leap::graphics::ITexture* leap::graphics::DirectXEngine::CreateTexture(const std::string& path)
{
	if (auto it{ m_pTextures.find(path) }; it != end(m_pTextures))
	{
		return it->second.get();
	}

	auto pTexture{ std::make_unique<DirectXTexture>(m_pDevice, path) };
	auto pTextureRaw{ pTexture.get() };

	m_pTextures[path] = std::move(pTexture);
	return pTextureRaw;
}

void leap::graphics::DirectXEngine::SetDirectionLight(const glm::mat3x3& transform)
{
	glm::mat4x3 lightTransform { transform };
	lightTransform[3] = glm::vec3{ m_pCamera->GetInverseViewMatrix()[3] } - transform[2] * 25.0f;

	m_DirectionalLight.SetTransform(lightTransform);

	for (const auto& pMaterial : m_pMaterials)
	{
		pMaterial.second->SetFloat3("gLightDirection", transform[2]);
	}
}

void leap::graphics::DirectXEngine::Release()
{
	if (m_pSwapChain)
	{
		m_pSwapChain->Release();
		m_pSwapChain = nullptr;
	}
	if (m_pDeviceContext)
	{
		m_pDeviceContext->ClearState();
		m_pDeviceContext->Flush();
		m_pDeviceContext->Release();
		m_pDeviceContext = nullptr;
	}
	if (m_pDevice)
	{
		m_pDevice->Release();
		m_pDevice = nullptr;
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}

void leap::graphics::DirectXEngine::ReloadDirectXEngine()
{
	// Release the previous version of the graphics engine
	Release();

	/// Create device & Device context
	UINT createDeviceFlags = 0;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;

#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	IDXGIFactory1* pFactory = nullptr;
	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&pFactory));
	HRESULT result{};

	if (SUCCEEDED(hr))
	{
		IDXGIAdapter1* pAdapter = nullptr;

		hr = pFactory->EnumAdapters1(0, &pAdapter);
		if (SUCCEEDED(hr))
		{
			result = D3D11CreateDevice(pAdapter, D3D_DRIVER_TYPE_UNKNOWN, nullptr, createDeviceFlags, &featureLevel, 1, D3D11_SDK_VERSION, &m_pDevice, nullptr, &m_pDeviceContext);

			if (FAILED(result)) throw std::runtime_error{ "DirectXEngine Error: Failed to create DirectX device" };

			pAdapter->Release();
		}

		pFactory->Release();
	}

	// Check if the anti aliasing level is supported
	UINT supportedLevels{};
	result = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, static_cast<UINT>(m_AntiAliasing), &supportedLevels);
	while (FAILED(result) || supportedLevels == 0)
	{
		m_AntiAliasing = static_cast<AntiAliasing>(static_cast<UINT>(m_AntiAliasing) / 2);
		result = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, static_cast<UINT>(m_AntiAliasing), &supportedLevels);
	}

	// Get the window size to use for the swap chain
	int width, height;
	glfwGetWindowSize(m_pWindow, &width, &height);

	IDXGIFactory1* pDxgiFactory{ nullptr };
	result = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&pDxgiFactory));
	if (FAILED(result)) throw std::runtime_error{ "DirectXEngine Error: Failed to create DirectX factory" };

	// Create swapchain
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 60;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = static_cast<UINT>(m_AntiAliasing);
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;
	swapChainDesc.OutputWindow = glfwGetWin32Window(m_pWindow);

	result = pDxgiFactory->CreateSwapChain(m_pDevice, &swapChainDesc, &m_pSwapChain);
	if (FAILED(result)) throw std::runtime_error{ "DirectXEngine Error: Failed to create DirectX swap chain" };

	// Create render target
	ID3D11Texture2D* pRenderTarget{};
	result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pRenderTarget));
	if (FAILED(result)) throw std::runtime_error{ "DirectXEngine Error: Failed to retrieve render target buffer from the swap chain" };

	DirectXRenderTarget::RTDesc renderTargetDesc{};
	renderTargetDesc.width = width;
	renderTargetDesc.height = height;
	renderTargetDesc.pOptionalColorBuffer = pRenderTarget;
	renderTargetDesc.depthFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	renderTargetDesc.hasDepthTexture = true;
	renderTargetDesc.hasColorTexture = true;
	renderTargetDesc.antiAliasing = m_AntiAliasing;

	m_RenderTarget.Create(m_pDevice, m_pDeviceContext, renderTargetDesc);

	/// Set viewport
	D3D11_VIEWPORT viewport{};
	viewport.Width = static_cast<FLOAT>(width);
	viewport.Height = static_cast<FLOAT>(height);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	m_pDeviceContext->RSSetViewports(1, &viewport);

	// Create a new shadow renderer using new video settings
	m_ShadowRenderer.Create(m_pDevice, m_pDeviceContext, m_ShadowRenderer.GetShadowMapSize());

	// Reload existing textures, materials & meshes using new video settings
	for (const auto& texturePair : m_pTextures)
	{
		texturePair.second->Reload(m_pDevice, texturePair.first);
	}

	for (const auto& materialPair : m_pMaterials)
	{
		materialPair.second->Reload(m_pDevice);

		// Reconnect the shadowmap to the material
		materialPair.second->SetTexture("gShadowMap", m_ShadowRenderer.GetShadowMap());
	}

	DirectXMeshLoader::GetInstance().Reload(m_pDevice);

	for (const auto& pRenderer : m_pRenderers)
	{
		pRenderer->Reload(m_pDevice, m_pDeviceContext);
	}

	std::cout << "DirectXRenderer Log: Successfully reloaded DirectX engine\n";

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOther(m_pWindow, true);
	ImGui_ImplDX11_Init(m_pDevice, m_pDeviceContext);
	ImGui::StyleColorsDark();
}

void leap::graphics::DirectXEngine::Draw()
{
	if (!m_IsInitialized) return;
	if (!m_pCamera) return;

	// Shadow pass
	// Unbind SRV
	constexpr ID3D11ShaderResourceView* const pSRV[] = { nullptr,nullptr };
	//m_pDeviceContext->PSSetShaderResources(1, 1, pSRV);
	m_pDeviceContext->PSSetShaderResources(0, 2, pSRV);

	// Set render target
	m_ShadowRenderer.SetupTarget();

	// Set the light view projection matrix
	m_ShadowRenderer.SetLightMatrix(m_DirectionalLight.GetViewProjection());

	// Render the shadow pass
	m_ShadowRenderer.Draw(m_pRenderers);

	// Camera pass
	// Set render target
	m_RenderTarget.Apply();

	// Clear target views
	const glm::vec4& clearColor = m_pCamera->GetColor();
	m_RenderTarget.Clear(clearColor);

	// Set camera matrix
	DirectXMaterial::SetViewProjectionMatrix(m_pCamera->GetProjectionMatrix() * m_pCamera->GetViewMatrix());

	// Apply the shadow map to each material
	for (const auto& pMaterial : m_pMaterials)
	{
		pMaterial.second->SetMat4x4("gLightViewProj", m_DirectionalLight.GetViewProjection());
	}

	// Render each mesh
	for (const auto& pRenderer : m_pRenderers)
	{
		pRenderer->Draw();
	}

	// Imgui render
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Swap render buffers
	m_pSwapChain->Present(0, 0);
}

void leap::graphics::DirectXEngine::GuiDraw()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
