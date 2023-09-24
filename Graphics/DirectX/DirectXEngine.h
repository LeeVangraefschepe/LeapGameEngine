#pragma once
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;
struct ID3D11Resource;
struct ID3D11RenderTargetView;

#include "../Interfaces/IRenderer.h"

#include "DirectXMeshRenderer.h"
#include "DirectXMaterial.h"

#include <vector>
#include <memory>

namespace leap::graphics
{
	class Camera;
	class IMeshRenderer;
	class IMaterial;

	class DirectXEngine final : public IRenderer
	{
	public:
		DirectXEngine(GLFWwindow* pWindow);
		~DirectXEngine() override;
		DirectXEngine(const DirectXEngine& other) = delete;
		DirectXEngine(DirectXEngine&& other) = delete;
		DirectXEngine& operator=(const DirectXEngine& other) = delete;
		DirectXEngine& operator=(DirectXEngine&& other) = delete;

		virtual void Initialize() override;
		virtual void Draw() override;
		virtual void SetActiveCamera(Camera* pCamera) override { m_pCamera = pCamera; }
		virtual Camera* GetCamera() const override { return m_pCamera; }
		virtual IMeshRenderer* CreateMeshRenderer();
		virtual void RemoveMeshRenderer(IMeshRenderer* pMeshRenderer);
		virtual IMaterial* CreateMaterial(std::unique_ptr<Shader, ShaderDelete> pShader) override;
		virtual void SetDirectionLight(const glm::vec3& direction) override;

	private:
		GLFWwindow* m_pWindow;
		ID3D11Device* m_pDevice{ nullptr };
		ID3D11DeviceContext* m_pDeviceContext{ nullptr };
		IDXGISwapChain* m_pSwapChain{ nullptr };
		ID3D11Texture2D* m_pDepthStencilBuffer{ nullptr };
		ID3D11DepthStencilView* m_pDepthStencilView{ nullptr };
		ID3D11Resource* m_pRenderTargetBuffer{ nullptr };
		ID3D11RenderTargetView* m_pRenderTargetView{ nullptr };

		std::vector<std::unique_ptr<DirectXMeshRenderer>> m_pRenderers{};
		std::vector<std::unique_ptr<DirectXMaterial>> m_pMaterials{};

		bool m_IsInitialized{};
		Camera* m_pCamera{};
	};
}