#pragma once
#include <vector>
#include "VulkanTypes.h"
#include "../Interfaces/IRenderer.h"

class GLFWwindow;
namespace leap::graphics
{
	class VulkanEngine final : public IRenderer
	{
	public:
		VulkanEngine(GLFWwindow* pWindow);
		virtual ~VulkanEngine();

		virtual void Initialize() override;
		virtual void Draw() override;

	private:
		// GLFW window
		GLFWwindow* m_pWindow{ nullptr };
		VkExtent2D m_WindowExtent{ 1280, 720 };

		// Engine stats
		bool m_IsInitialized{ false };
		int m_FrameNumber{ 0 };

		// Vulkan
		VkInstance m_Instance{ VK_NULL_HANDLE }; // Vulkan instance handle
		VkDebugUtilsMessengerEXT m_DebugMessenger{ VK_NULL_HANDLE }; // Vulkan debug output handle
		VkPhysicalDevice m_PhysicalDevice{ VK_NULL_HANDLE }; // Vulkan physical device handle
		VkDevice m_Device{ VK_NULL_HANDLE }; // Vulkan logical device handle
		VkSurfaceKHR m_Surface{ VK_NULL_HANDLE }; // Vulkan surface handle

		// Swap chain
		VkSwapchainKHR m_SwapChain{ VK_NULL_HANDLE };
		VkFormat m_SwapChainImageFormat{ VK_FORMAT_UNDEFINED };
		std::vector<VkImage> m_SwapChainImages;
		std::vector<VkImageView> m_SwapChainImageViews;

		void Cleanup();
		void InitializeVulkan();
		void InitializeSwapChain();
	};
}
