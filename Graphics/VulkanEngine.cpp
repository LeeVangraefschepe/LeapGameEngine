#include "VulkanEngine.h"

// GLFW Window
#include <glfw3.h>

// Bootstrap library
#include "VkBootstrap.h"

// STL includes
#include <iostream>

#pragma region macros
// Vulkan error macro
using namespace std;
#define VK_CHECK(x)													\
	do																\
	{																\
		VkResult err = x;											\
		if (err)													\
		{															\
			std::cout << "Detected Vulkan error: " << err << "\n";	\
			abort();												\
		}															\
	} while (0)
#pragma endregion

// Vulkan includes
#include "VulkanTypes.h"
#include "VulkanInitializers.h"

VulkanEngine::VulkanEngine(GLFWwindow* pWindow)
	: m_pWindow{ pWindow }
{}

void VulkanEngine::Initialize()
{
	// Load the core Vulkan structures
	InitializeVulkan();

	m_IsInitialized = true;
	std::cout << "VulkanEngine initialized\n";
}

void VulkanEngine::Draw()
{
	
}

void VulkanEngine::Cleanup()
{

}

void VulkanEngine::InitializeVulkan()
{
	vkb::InstanceBuilder builder;

	// Make the Vulkan instance, with basic debug features
	auto instanceDesc = builder.set_app_name("Leap Vulkan Engine")
		.request_validation_layers(true)
		.require_api_version(1, 1, 0)
		.use_default_debug_messenger()
		.build();

	vkb::Instance vkbInstance = instanceDesc.value();

	// Store the instance
	m_Instance = vkbInstance.instance;

	// Store the debug messenger
	m_DebugMessenger = vkbInstance.debug_messenger;
}