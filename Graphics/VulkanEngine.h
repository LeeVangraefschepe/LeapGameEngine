#pragma once
#include <vector>
#include <deque>
#include <functional>
#include "VulkanTypes.h"

class GLFWwindow;
namespace leap::graphics
{
	struct DeletionQueue
	{
		std::deque <std::function<void()>> deletors;

		void Flush()
		{
			for (auto it = deletors.rbegin(); it != deletors.rend(); it++)
				(*it)();

			deletors.clear();
		}
	};

	class VulkanEngine final
	{
	public:
		VulkanEngine(GLFWwindow* pWindow);
		~VulkanEngine();

		void Initialize();
		void Draw();
		void Cleanup();

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
		void InitializeVulkan();

		// Swap chain
		VkSwapchainKHR m_SwapChain{ VK_NULL_HANDLE };
		VkFormat m_SwapChainImageFormat{ VK_FORMAT_UNDEFINED };
		std::vector<VkImage> m_SwapChainImages;
		std::vector<VkImageView> m_SwapChainImageViews;

		void InitializeSwapChain();

		// Queues & buffers
		VkQueue m_GraphicsQueue{ VK_NULL_HANDLE };
		uint32_t m_GraphicsQueueFamily{ UINT32_MAX };
		VkCommandPool m_CommandPool{ VK_NULL_HANDLE };
		VkCommandBuffer m_MainCommandBuffer{ VK_NULL_HANDLE };
		void InitCommands();

		// Render passes
		VkRenderPass m_RenderPass{ VK_NULL_HANDLE };
		std::vector<VkFramebuffer> m_Framebuffers;
		void InitializeDefaultRenderPass();
		void InitializeFramebuffers();

		// Render loop
		VkSemaphore m_PresentSemaphore{ VK_NULL_HANDLE };
		VkSemaphore m_RenderSemaphore{ VK_NULL_HANDLE };
		VkFence m_RenderFence{ VK_NULL_HANDLE };
		void InitializeSyncStructures();

		// Shaders
		bool LoadShaderModule(const char* filePath, VkShaderModule* outShaderModule);
		void InitializePipelines();

		// Graphics pipeline
		VkPipelineLayout m_TrianglePipelineLayout{ VK_NULL_HANDLE };
		VkPipeline m_TrianglePipeline{};

		// Deletion
		DeletionQueue m_MainDeletionQueue;
	};
}
