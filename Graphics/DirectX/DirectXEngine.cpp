#include "glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw3native.h"
#include "DirectXEngine.h"
#include "DirectXTex.h"

#include <glm.hpp>

#include "../Camera.h"
#include "Debug.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dx11effect.h>

#include "DirectXShaderReader.h"
#include "DirectXTexture.h"
#include "DirectXMeshLoader.h"
#include "DirectXMaterial.h"
#include "DirectXDefaults.h"

#include "../Shaders/PosNormTex3D.h"

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw.h"
#include "imgui_impl_dx11.h"

#include "../Data/CustomMesh.h"

#include <sstream>

leap::graphics::DirectXEngine::DirectXEngine(GLFWwindow* pWindow) : m_pWindow(pWindow)
{
	Debug::Log("DirectXRenderer Log: Created DirectX engine");
}

leap::graphics::DirectXEngine::~DirectXEngine()
{
	Release();
	Debug::Log("DirectXRenderer Log: Destroyed DirectX engine");
}

void leap::graphics::DirectXEngine::Initialize()
{
	CreateDirectXEngine();

	Debug::Log("DirectXRenderer Log: Creating default material with ID \"Default\"");
	CreateMaterial(shaders::PosNormTex3D::GetShader(), "Default", true);

	Debug::Log("DirectXRenderer Log: Creating debug renderer");
	m_pDebugRenderer = CreateMeshRenderer();
	m_pDebugRenderer->SetIsLineRenderer(true);
	m_pDebugMesh = CreateMesh();
	m_pDebugRenderer->SetMesh(m_pDebugMesh);

	m_IsInitialized = true;
	Debug::Log("DirectXRenderer Log: Successfully initialized DirectX engine");
}

void leap::graphics::DirectXEngine::Draw()
{
	if (!m_IsInitialized) return;

	if (m_pCamera)
	{
		if (!m_DebugDrawings.GetIndexBuffer().empty())
		{
			m_pDebugMesh->ReloadMesh(m_DebugDrawings);
			m_DebugDrawings.Clear();
		}

		RenderCameraView();
	}
	else
	{
		SetupNonCameraView();
	}

	// Render sprites
	m_SpriteRenderer.Draw();

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

void leap::graphics::DirectXEngine::SetAntiAliasing(AntiAliasing antiAliasing)
{
	m_AntiAliasing = antiAliasing;

	if (m_IsInitialized) CreateRenderTargetAndSetViewport();
}

void leap::graphics::DirectXEngine::SetWindowSize(const glm::ivec2&)
{
	if (m_IsInitialized) CreateRenderTargetAndSetViewport();
}

void leap::graphics::DirectXEngine::SetShadowMapData(unsigned int shadowMapWidth, unsigned int shadowMapHeight, float orthoSize, float nearPlane, float farPlane)
{
	m_DirectionalLight.SetShadowMapData(static_cast<float>(shadowMapWidth) / shadowMapHeight, orthoSize, nearPlane, farPlane);
	m_ShadowRenderer.Create(this, { shadowMapWidth, shadowMapHeight });
}

void leap::graphics::DirectXEngine::SetDirectionLight(const glm::mat3x3& transform)
{
	if (!m_pCamera) return;

	glm::mat4x3 lightTransform{ transform };
	lightTransform[3] = glm::vec3{ m_pCamera->GetInverseViewMatrix()[3] } - transform[2] * 25.0f;

	m_DirectionalLight.SetTransform(lightTransform);

	const auto& lightDir{ transform[2] };
	for (const auto& pMaterial : m_pMaterials)
	{
		pMaterial.second->SetFloat3("gLightDirection", lightDir);
		pMaterial.second->SetMat4x4("gLightViewProj", m_DirectionalLight.GetViewProjection());
	}
	for (const auto& pMaterial : m_pUniqueMaterials)
	{
		pMaterial->SetFloat3("gLightDirection", lightDir);
		pMaterial->SetMat4x4("gLightViewProj", m_DirectionalLight.GetViewProjection());
	}
}

leap::graphics::IMeshRenderer* leap::graphics::DirectXEngine::CreateMeshRenderer()
{
	m_pRenderers.push_back(std::make_unique<DirectXMeshRenderer>(this));
	return m_pRenderers[m_pRenderers.size() - 1].get();
}

void leap::graphics::DirectXEngine::RemoveMeshRenderer(IMeshRenderer* pMeshRenderer)
{
	m_pRenderers.erase(std::remove_if(begin(m_pRenderers), end(m_pRenderers), [pMeshRenderer](const auto& pRenderer) { return pMeshRenderer == pRenderer.get(); }));
}

leap::graphics::IMesh* leap::graphics::DirectXEngine::CreateMesh(const std::string& path, bool cached)
{
	if (!cached)
	{
		m_pUniqueMeshes.emplace_back(std::make_unique<DirectXMesh>(this, path));
		return m_pUniqueMeshes[m_pUniqueMeshes.size() - 1].get();
	}

	if (!m_pMeshes.contains(path))
	{
		m_pMeshes[path] = std::make_unique<DirectXMesh>(this, path);
	}

	return m_pMeshes[path].get();
}

leap::graphics::IMesh* leap::graphics::DirectXEngine::CreateMesh()
{
	m_pUniqueMeshes.emplace_back(std::make_unique<DirectXMesh>(this));
	return m_pUniqueMeshes[m_pUniqueMeshes.size() - 1].get();
}

void leap::graphics::DirectXEngine::RemoveMesh(IMesh* pMesh)
{
	std::erase_if(m_pUniqueMeshes, [pMesh](const auto& pUniqueMesh) { return pUniqueMesh.get() == pMesh; });
}

void leap::graphics::DirectXEngine::AddSprite(Sprite* pSprite)
{
	m_SpriteRenderer.AddSprite(pSprite);
}

void leap::graphics::DirectXEngine::RemoveSprite(Sprite* pSprite)
{
	m_SpriteRenderer.RemoveSprite(pSprite);
}

leap::graphics::IMaterial* leap::graphics::DirectXEngine::CreateMaterial(std::unique_ptr<Shader, ShaderDelete> pShader, const std::string& name, bool cached)
{
	if (cached)
	{
		if (auto it{ m_pMaterials.find(name) }; it != end(m_pMaterials))
		{
			return it->second.get();
		}
	}

	const DirectXShader shader{ DirectXShaderReader::GetShaderData(std::move(pShader)) };
	auto pMaterial{ std::make_unique<DirectXMaterial>(this, shader.path, shader.vertexDataFunction) };
	auto pMaterialRaw{ pMaterial.get() };

	pMaterial->SetTexture("gShadowMap", m_ShadowRenderer.GetShadowMap());
	pMaterial->SetFloat3("gLightDirection", m_DirectionalLight.GetDirection());
	pMaterial->SetMat4x4("gLightViewProj", m_DirectionalLight.GetViewProjection());

	if (cached)
	{
		m_pMaterials[name] = std::move(pMaterial);
	}
	else 
	{
		m_pUniqueMaterials.emplace_back(std::move(pMaterial));
	}
	return pMaterialRaw;
}

leap::graphics::IMaterial* leap::graphics::DirectXEngine::CloneMaterial(const std::string& original, const std::string& clone, bool cached)
{
	if (cached)
	{
		if (auto it{ m_pMaterials.find(clone) }; it != end(m_pMaterials))
		{
			return it->second.get();
		}
	}

	if (auto it{ m_pMaterials.find(original) }; it != end(m_pMaterials))
	{
		std::unique_ptr<DirectXMaterial> pMaterial{ it->second->Clone() };
		auto pMaterialRaw{ pMaterial.get() };

		pMaterial->SetTexture("gShadowMap", m_ShadowRenderer.GetShadowMap());
		pMaterial->SetFloat3("gLightDirection", m_DirectionalLight.GetDirection());
		pMaterial->SetMat4x4("gLightViewProj", m_DirectionalLight.GetViewProjection());

		if (cached)
		{
			m_pMaterials[clone] = std::move(pMaterial);
		}
		else
		{
			m_pUniqueMaterials.emplace_back(std::move(pMaterial));
		}
		return pMaterialRaw;
	}

	return nullptr;
}

void leap::graphics::DirectXEngine::RemoveMaterial(IMaterial* pMaterial)
{
	std::erase_if(m_pUniqueMaterials, [pMaterial](const auto& pUniqueMaterial) { return pUniqueMaterial.get() == pMaterial; });
}

leap::graphics::ITexture* leap::graphics::DirectXEngine::CreateTexture(const std::string& path, bool cached)
{
	if (!cached)
	{
		m_pUniqueTextures.emplace_back(std::make_unique<DirectXTexture>(this, path));
		return m_pUniqueTextures[m_pUniqueTextures.size() - 1].get();
	}

	if (!m_pTextures.contains(path))
	{
		m_pTextures[path] = std::make_unique<DirectXTexture>(this, path);
	}

	return m_pTextures[path].get();
}

leap::graphics::ITexture* leap::graphics::DirectXEngine::CreateTexture(int width, int height)
{
	m_pUniqueTextures.emplace_back(std::make_unique<DirectXTexture>(this, width, height));
	return m_pUniqueTextures[m_pUniqueTextures.size() - 1].get();
}

void leap::graphics::DirectXEngine::RemoveTexture(ITexture* pTexture)
{
	std::erase_if(m_pUniqueTextures, [pTexture](const auto& pUniqueTexture) { return pUniqueTexture.get() == pTexture; });
}

void leap::graphics::DirectXEngine::DrawLines(const std::vector<std::pair<glm::vec3, glm::vec3>>& lines)
{
	unsigned int index{ static_cast<unsigned int>(m_DebugDrawings.GetIndexBuffer().size()) };
	for (const auto& line : lines)
	{
		m_DebugDrawings.AddVertex(line.first);
		m_DebugDrawings.AddVertex(line.second);
		m_DebugDrawings.AddIndex(index++);
		m_DebugDrawings.AddIndex(index++);
	}
}

void leap::graphics::DirectXEngine::DrawLine(const glm::vec3& start, const glm::vec3& end)
{
	unsigned int index{ static_cast<unsigned int>(m_DebugDrawings.GetIndexBuffer().size()) };
	m_DebugDrawings.AddVertex(start);
	m_DebugDrawings.AddVertex(end);
	m_DebugDrawings.AddIndex(index++);
	m_DebugDrawings.AddIndex(index++);
}

void leap::graphics::DirectXEngine::Release()
{
	m_pRenderers.clear();
	m_pMaterials.clear();
	m_pTextures.clear();
	m_pUniqueTextures.clear();
	m_pMeshes.clear();
	m_pUniqueMeshes.clear();

	ReleaseSwapchain();
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

void leap::graphics::DirectXEngine::ReleaseSwapchain()
{
	if (m_pSwapChain)
	{
		m_pSwapChain->Release();
		m_pSwapChain = nullptr;
	}
}

void leap::graphics::DirectXEngine::CreateDirectXEngine()
{
	/// Create device & Device context
	UINT createDeviceFlags = 0;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;

#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	IDXGIFactory1* pFactory = nullptr;
	HRESULT result{ CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&pFactory)) };

	if (SUCCEEDED(result))
	{
		IDXGIAdapter1* pAdapter = nullptr;

		result = pFactory->EnumAdapters1(0, &pAdapter);
		if (SUCCEEDED(result))
		{
			result = D3D11CreateDevice(pAdapter, D3D_DRIVER_TYPE_UNKNOWN, nullptr, createDeviceFlags, &featureLevel, 1, D3D11_SDK_VERSION, &m_pDevice, nullptr, &m_pDeviceContext);

			if (FAILED(result)) Debug::LogError("DirectXEngine Error: Failed to create DirectX device");

			pAdapter->Release();
		}

		pFactory->Release();
	}

	// Create swap chain and viewport
	CreateRenderTargetAndSetViewport();

	// Create a new shadow renderer using new video settings
	m_ShadowRenderer.Create(this, m_ShadowRenderer.GetShadowMapSize());

	Debug::Log("DirectXRenderer Log: Successfully created DirectX engine");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOther(m_pWindow, true);
	ImGui_ImplDX11_Init(m_pDevice, m_pDeviceContext);
	ImGui::StyleColorsDark();
}

void leap::graphics::DirectXEngine::CreateRenderTargetAndSetViewport()
{
	// Release previous versions of the swapchain
	ReleaseSwapchain();

	// Get the window size to use for the swap chain
	int width, height;
	glfwGetWindowSize(m_pWindow, &width, &height);

	// Create factory
	IDXGIFactory1* pDxgiFactory{ nullptr };
	HRESULT result = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&pDxgiFactory));
	if (FAILED(result)) Debug::LogError("DirectXEngine Error: Failed to create DirectX factory");

	// Check if the anti aliasing level is supported
	UINT supportedLevels{};
	result = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, static_cast<UINT>(m_AntiAliasing), &supportedLevels);
	while (FAILED(result) || supportedLevels == 0)
	{
		m_AntiAliasing = static_cast<AntiAliasing>(static_cast<UINT>(m_AntiAliasing) / 2);
		result = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, static_cast<UINT>(m_AntiAliasing), &supportedLevels);
	}

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
	if (FAILED(result)) Debug::LogError("DirectXEngine Error: Failed to create DirectX swap chain");

	// Create render target
	ID3D11Texture2D* pRenderTarget{};
	result = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pRenderTarget));
	if (FAILED(result)) Debug::LogError("DirectXEngine Error: Failed to retrieve render target buffer from the swap chain");

	DirectXRenderTarget::RTDesc renderTargetDesc{};
	renderTargetDesc.width = width;
	renderTargetDesc.height = height;
	renderTargetDesc.pOptionalColorBuffer = pRenderTarget;
	renderTargetDesc.depthFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	renderTargetDesc.hasDepthTexture = true;
	renderTargetDesc.hasColorTexture = true;
	renderTargetDesc.antiAliasing = m_AntiAliasing;

	m_RenderTarget.Create(m_pDevice, m_pDeviceContext, renderTargetDesc);

	// Set viewport
	D3D11_VIEWPORT viewport{};
	viewport.Width = static_cast<FLOAT>(width);
	viewport.Height = static_cast<FLOAT>(height);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	m_pDeviceContext->RSSetViewports(1, &viewport);

	// Create a new sprite renderer using new video settings
	m_SpriteRenderer.Create(this, glm::vec2{ width, height });

	Debug::Log("DirectXRenderer Log: Successfully created swapchain, rendertarget and spriterenderer");
}

void leap::graphics::DirectXEngine::RenderCameraView() const
{
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

	// Render each mesh
	for (const auto& pRenderer : m_pRenderers)
	{
		pRenderer->Draw();
	}
}

void leap::graphics::DirectXEngine::SetupNonCameraView() const
{
	// Unbind SRV
	constexpr ID3D11ShaderResourceView* const pSRV[] = { nullptr,nullptr };
	//m_pDeviceContext->PSSetShaderResources(1, 1, pSRV);
	m_pDeviceContext->PSSetShaderResources(0, 2, pSRV);

	// Set render target
	m_RenderTarget.Apply();

	// Clear target views
	constexpr glm::vec4 clearColor{};
	m_RenderTarget.Clear(clearColor);
}

void leap::graphics::DirectXEngine::PrintDiagnostics() const
{
	std::stringstream ss{};

	ss << "\n";
	ss << "DirectX Engine Usage:";
	ss << "Meshes cached " << m_pMeshes.size() << "\n";
	ss << "Meshes non-cached " << m_pUniqueMeshes.size() << "\n";
	ss << "Materials " << m_pMaterials.size() << "\n";
	ss << "Unique Materials " << m_pUniqueMaterials.size() << "\n";
	ss << "Mesh renderers " << m_pRenderers.size() << "\n";
	ss << "Textures cached " << m_pTextures.size() << "\n";
	ss << "Textured non-cached " << m_pUniqueTextures.size() << "\n";
	ss << "\n";

	Debug::Log(ss.str());
}
