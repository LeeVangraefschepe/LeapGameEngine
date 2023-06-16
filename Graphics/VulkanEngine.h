#pragma once

#include "VulkanTypes.h"

class GLFWwindow;
class VulkanEngine final
{
public:
	VulkanEngine(GLFWwindow* pWindow);

	void Initialize();
	void Draw();
	void Cleanup();

private:
	// GLFW window
	GLFWwindow* m_pWindow{ nullptr };

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
};