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

	struct GPUCameraData
	{
		glm::mat4 view;
		glm::mat4 proj;
		glm::mat4 viewProj;
		glm::mat4 viewInv;
	};

	struct GPUSceneData
	{
		glm::vec4 fogColor;
		glm::vec4 fogDistance;
		glm::vec4 ambientColor; // w is ambient power
		glm::vec4 lightDir; // w is light power
		glm::vec4 lightColor;
	};

	struct GPUObjectData
	{
		glm::mat4 world;
	};

	struct FrameData
	{
		// Sync objects
		VkSemaphore presentSemaphore{ VK_NULL_HANDLE }, renderSemaphore{ VK_NULL_HANDLE };
		VkFence renderFence{ VK_NULL_HANDLE };

		// Commands
		VkCommandPool commandPool{ VK_NULL_HANDLE };
		VkCommandBuffer mainCommandBuffer{ VK_NULL_HANDLE };

		// Buffers
		AllocatedBuffer cameraBuffer;
		AllocatedBuffer objectBuffer;

		// Descriptors
		VkDescriptorSet globalDescriptor;
		VkDescriptorSet objectDescriptor;
	};

	// Number of frames that can be processed concurrently
	constexpr uint8_t MAX_FRAMES_IN_FLIGHT = 2;

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
		VkPhysicalDeviceProperties m_DeviceProperties;
		void InitCommands();
		AllocatedBuffer CreateBuffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

		// Descriptor sets
		VkDescriptorSetLayout m_GlobalSetLayout{ VK_NULL_HANDLE };
		VkDescriptorSetLayout m_ObjectSetLayout{ VK_NULL_HANDLE };

		VkDescriptorPool m_DescriptorPool{ VK_NULL_HANDLE };
		void InitDescriptors();

		// Render passes
		VkRenderPass m_RenderPass{ VK_NULL_HANDLE };
		std::vector<VkFramebuffer> m_Framebuffers;
		void InitializeDefaultRenderPass();
		void InitializeFramebuffers();

		// Render loop
		FrameData m_Frames[MAX_FRAMES_IN_FLIGHT];
		FrameData& GetCurrentFrame();
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

		GPUSceneData m_GPUSceneData;
		AllocatedBuffer m_SceneDataBuffer;
		size_t PadUniformBufferSize(size_t originalSize);

		Material* CreateMaterial(const std::string& name, VkPipeline pipeline, VkPipelineLayout pipelineLayout);
		Material* GetMaterial(const std::string& name);
		Mesh* GetMesh(const std::string& name);
		void DrawObjects(VkCommandBuffer cmd, RenderObject* first, int count);

		void InitializeScene();
	};
}