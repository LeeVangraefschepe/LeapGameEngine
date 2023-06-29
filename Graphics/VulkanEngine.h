#pragma once
#include <vector>
#include <deque>
#include <unordered_map>
#include <string>
#include <functional>

// GLM
#include <vec4.hpp>
#include <mat4x4.hpp>

#include "vk_mesh.h"
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

	struct MeshPushConstants
	{
		glm::vec4 data;
		glm::mat4 wvp;
	};

	struct Material
	{
		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout;
	};

	struct RenderObject
	{
		Mesh* mesh;
		Material* material;
		glm::mat4 transform;
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
		VkPipelineLayout m_MeshPipelineLayout{ VK_NULL_HANDLE };
		VkPipeline m_MeshPipeline{ VK_NULL_HANDLE };

		// Deletion
		DeletionQueue m_MainDeletionQueue;

		// Allocator
		VmaAllocator m_Allocator{ VK_NULL_HANDLE };

		// Mesh
		Mesh m_TeapotMesh;
		void LoadMeshes();
		void UploadMesh(Mesh& mesh);

		// Depth buffer
		VkImageView m_DepthImageView{ VK_NULL_HANDLE };
		AllocatedImage m_DepthImage;
		VkFormat m_DepthFormat;

		// Scene management
		std::vector<RenderObject> m_Renderables;

		std::unordered_map<std::string, Material> m_Materials;
		std::unordered_map<std::string, Mesh> m_Meshes;

		Material* CreateMaterial(const std::string& name, VkPipeline pipeline, VkPipelineLayout pipelineLayout);
		Material* GetMaterial(const std::string& name);
		Mesh* GetMesh(const std::string& name);
		void DrawObjects(VkCommandBuffer cmd, RenderObject* first, int count);

		void InitializeScene();
	};
}