#include "VulkanEngine.h"

// GLFW Window
#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>

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
#include "VulkanInitializers.h"

using namespace leap::graphics;

VulkanEngine::VulkanEngine(GLFWwindow* pWindow)
	: m_pWindow{ pWindow }
{}

VulkanEngine::~VulkanEngine()
{
	Cleanup();
}

void VulkanEngine::Initialize()
{
	// Load the core Vulkan structures
	InitializeVulkan();
	InitializeSwapChain();
	InitCommands();
	InitializeDefaultRenderPass();
	InitializeFramebuffers();
	InitializeSyncStructures();

	m_IsInitialized = true;
	std::cout << "VulkanEngine initialized\n";
}

void VulkanEngine::Draw()
{
	// Wait for the GPU to be done with the last frame, 1s timeout
	VK_CHECK(vkWaitForFences(m_Device, 1, &m_RenderFence, true, 1000000000));
	VK_CHECK(vkResetFences(m_Device, 1, &m_RenderFence));

	// Request image from the swap chain, wait 1 second if none available
	uint32_t swapchainImageIndex;
	VK_CHECK(vkAcquireNextImageKHR(m_Device, m_SwapChain, 1000000000, m_PresentSemaphore, nullptr, &swapchainImageIndex));

	// Commands have finished executing, so reset the buffer
	VK_CHECK(vkResetCommandBuffer(m_MainCommandBuffer, 0));

	// Record commands
	VkCommandBufferBeginInfo cmdBeginInfo{};
	cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBeginInfo.pNext = nullptr;

	cmdBeginInfo.pInheritanceInfo = nullptr;
	// We record each frame to the command buffer, so we need to let Vulkan know that we're only executing it once
	cmdBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	// Begin recording
	VK_CHECK(vkBeginCommandBuffer(m_MainCommandBuffer, &cmdBeginInfo));

	// Flashing blue clear color
	VkClearValue clearValue;
	float flash = abs(sin(m_FrameNumber / 120.f));
	clearValue.color = { {0.0f, flash, 0.0f, 1.0f} };

	// Begin the render pass
	VkRenderPassBeginInfo rpInfo{};
	rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rpInfo.pNext = nullptr;

	// Use the default render pass on the image with the index that we acquired
	rpInfo.renderPass = m_RenderPass;
	rpInfo.renderArea.offset.x = 0;
	rpInfo.renderArea.offset.y = 0;
	rpInfo.renderArea.extent = m_WindowExtent;
	rpInfo.framebuffer = m_Framebuffers[swapchainImageIndex];

	// Connect clear values
	rpInfo.clearValueCount = 1;
	rpInfo.pClearValues = &clearValue;

	// Begin the render pass
	vkCmdBeginRenderPass(m_MainCommandBuffer, &rpInfo, VK_SUBPASS_CONTENTS_INLINE);

	// RENDER STUFF HERE

	// End the render pass
	vkCmdEndRenderPass(m_MainCommandBuffer);

	// End recording
	VK_CHECK(vkEndCommandBuffer(m_MainCommandBuffer));

	// Prepare to submit to the queue
	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	VkSubmitInfo submit{};
	submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit.pNext = nullptr;

	submit.pWaitDstStageMask = &waitStage;

	// Wait for image to be ready for rendering before rendering to it
	submit.waitSemaphoreCount = 1;
	submit.pWaitSemaphores = &m_PresentSemaphore;

	// Signal ready when finished rendering
	submit.signalSemaphoreCount = 1;
	submit.pSignalSemaphores = &m_RenderSemaphore;

	submit.commandBufferCount = 1;
	submit.pCommandBuffers = &m_MainCommandBuffer;

	// Submit to the graphics queue
	VK_CHECK(vkQueueSubmit(m_GraphicsQueue, 1, &submit, m_RenderFence));

	// Present the image to the window when the rendering semaphore has signaled that rendering is done
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = nullptr;

	presentInfo.pSwapchains = &m_SwapChain;
	presentInfo.swapchainCount = 1;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &m_RenderSemaphore;

	// Which image to present
	presentInfo.pImageIndices = &swapchainImageIndex;

	VK_CHECK(vkQueuePresentKHR(m_GraphicsQueue, &presentInfo));

	m_FrameNumber++;
}

void VulkanEngine::Cleanup()
{
	if (m_IsInitialized)
	{
		// Wait for the GPU to be done with the last frame, 1s timeout
		vkWaitForFences(m_Device, 1, &m_RenderFence, true, 1000000000);

		vkDestroySemaphore(m_Device, m_RenderSemaphore, nullptr);
		vkDestroySemaphore(m_Device, m_PresentSemaphore, nullptr);
		vkDestroyFence(m_Device, m_RenderFence, nullptr);

		vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);

		vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);
		vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);

		// destroy swap chain resources and framebuffers
		for (int i = 0; i < m_SwapChainImages.size(); i++)
		{
			vkDestroyFramebuffer(m_Device, m_Framebuffers[i], nullptr);
			vkDestroyImageView(m_Device, m_SwapChainImageViews[i], nullptr);
		}

		vkDestroyDevice(m_Device, nullptr);
		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
		vkb::destroy_debug_utils_messenger(m_Instance, m_DebugMessenger);
		vkDestroyInstance(m_Instance, nullptr);
	}
}

void VulkanEngine::InitializeVulkan()
{
	std::cout << "Initializing Vulkan\n";
	vkb::InstanceBuilder builder;

	// Make the Vulkan instance, with basic debug features
	auto instanceDesc = builder.set_app_name("Leap Vulkan Engine")
		.request_validation_layers(true)
		.require_api_version(1, 1, 0)
		.use_default_debug_messenger()
		.request_validation_layers(true)
		.build();

	vkb::Instance vkbInstance = instanceDesc.value();

	// Store the instance
	m_Instance = vkbInstance.instance;

	// Store the debug messenger
	m_DebugMessenger = vkbInstance.debug_messenger;

	if (glfwCreateWindowSurface(m_Instance, m_pWindow, nullptr, &m_Surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}

	// Get the window size to use for the swap chain
	int width, height;
	glfwGetWindowSize(m_pWindow, &width, &height);
	m_WindowExtent.width = static_cast<uint32_t>(width);
	m_WindowExtent.height = static_cast<uint32_t>(height);

	// use vkbootstrap to select a GPU.
	// We want a GPU that can write to the SDL surface and supports Vulkan 1.1
	vkb::PhysicalDeviceSelector selector{ vkbInstance };
	vkb::PhysicalDevice physicalDevice = selector
		.set_minimum_version(1, 1)
		.set_surface(m_Surface)
		.select()
		.value();

	// create the final Vulkan device
	vkb::DeviceBuilder deviceBuilder{ physicalDevice };

	vkb::Device vkbDevice = deviceBuilder.build().value();

	// Get the VkDevice handle used in the rest of a Vulkan application
	m_Device = vkbDevice.device;
	m_PhysicalDevice = physicalDevice.physical_device;

	// Get the graphics queue for the rest of the Vulkan application
	m_GraphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
	m_GraphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();
}

void VulkanEngine::InitializeSwapChain()
{
	std::cout << "Initializing Swap Chain\n";

	vkb::SwapchainBuilder swapChainBuilder{ m_PhysicalDevice, m_Device, m_Surface };

	vkb::Swapchain vkbSwapChain = swapChainBuilder
		.use_default_format_selection()
		// use VSync present mode
		.set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
		.set_desired_extent(m_WindowExtent.width, m_WindowExtent.height)
		.build()
		.value();

	// store swap chain and its related images
	m_SwapChain = vkbSwapChain.swapchain;
	m_SwapChainImages = vkbSwapChain.get_images().value();
	m_SwapChainImageViews = vkbSwapChain.get_image_views().value();

	m_SwapChainImageFormat = vkbSwapChain.image_format;
}

void VulkanEngine::InitCommands()
{
	std::cout << "Initializing Commands\n";

	auto commandPoolInfo = vkinit::CommandPoolCreateInfo(m_GraphicsQueueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	VK_CHECK (vkCreateCommandPool(m_Device, &commandPoolInfo, nullptr, &m_CommandPool));

	auto commandBufferInfo = vkinit::CommandBufferAllocateInfo(m_CommandPool, 1);
	VK_CHECK(vkAllocateCommandBuffers(m_Device, &commandBufferInfo, &m_MainCommandBuffer));
}

void VulkanEngine::InitializeDefaultRenderPass()
{
	std::cout << "Initializing Default Render Pass\n";

	// Description of the image that we will be writing rendering commands to
	VkAttachmentDescription colorAttachment{};

	// format should be the same as the swap chain images
	colorAttachment.format = m_SwapChainImageFormat;
	// MSAA samples, set to 1 (no MSAA) by default
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	// Clear when render pass begins
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	// Keep the attachment stored when render pass ends
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	// Don't care about stencil data
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	// Image data layout before render pass starts (undefined = don't care)
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	// Image data layout after render pass (to change to), set to present by default
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	// Attachment number will index into the pAttachments array in the parent render pass itself
	colorAttachmentRef.attachment = 0;
	// Optimal layout for writing to the image
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// Create one sub pass (minimum one sub pass required)
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

	// Connect the color attachment description to the info
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;

	// Connect the subpass(es) to the info
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	VK_CHECK(vkCreateRenderPass(m_Device, &renderPassInfo, nullptr, &m_RenderPass));
}

void VulkanEngine::InitializeFramebuffers()
{
	std::cout << "Initializing Framebuffers\n";

	// Create the framebuffers for the swap chain images.
	// This will connect the render pass to the images for rendering
	VkFramebufferCreateInfo fbInfo{};
	fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

	// Connect the render pass to the framebuffer
	fbInfo.renderPass = m_RenderPass;
	fbInfo.attachmentCount = 1;
	fbInfo.width = m_WindowExtent.width;
	fbInfo.height = m_WindowExtent.height;
	fbInfo.layers = 1;

	// Get the number of swap chain image views
	const uint32_t swapChainImageViewCount = static_cast<uint32_t>(m_SwapChainImageViews.size());
	m_Framebuffers = std::vector<VkFramebuffer>(swapChainImageViewCount);

	// Create the framebuffers for each image view
	for (uint32_t i = 0; i < swapChainImageViewCount; i++)
	{
		fbInfo.pAttachments = &m_SwapChainImageViews[i];
		VK_CHECK(vkCreateFramebuffer(m_Device, &fbInfo, nullptr, &m_Framebuffers[i]));
	}
}

void VulkanEngine::InitializeSyncStructures()
{
	std::cout << "Initializing Sync Structures\n";

	VkFenceCreateInfo fenceCreateInfo{};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.pNext = nullptr;

	// We want to create the fence with the Create Signaled flag,
	// so we can wait on it before using it on a GPU command (for the first frame)
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VK_CHECK(vkCreateFence(m_Device, &fenceCreateInfo, nullptr, &m_RenderFence));

	// Sempahore needs no flags
	VkSemaphoreCreateInfo semaphoreCreateInfo{};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.pNext = nullptr;
	semaphoreCreateInfo.flags = 0;

	VK_CHECK(vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_PresentSemaphore));
	VK_CHECK(vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_RenderSemaphore));
}