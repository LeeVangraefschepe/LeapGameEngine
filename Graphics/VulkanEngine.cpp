#include "VulkanEngine.h"

// VMA
#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

// GLFW Window
#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>

// GLM
#include <gtx/transform.hpp>

// Bootstrap library
#include <VkBootstrap.h>

// STL includes
#include <iostream>
#include <fstream>

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
#include "VulkanPipelineBuilder.h"


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
	InitializePipelines();
	LoadMeshes();

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

	// Clear color
	VkClearValue clearValue;
	clearValue.color = { {0.1f, 0.1f, 0.85f, 1.0f} };

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
	// ...

	// Bind the graphics pipeline
	vkCmdBindPipeline(m_MainCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_MeshPipeline);

	// Bind the vertex buffer with an offset of 0
	VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(m_MainCommandBuffer, 0, 1, &m_TeapotMesh.vertexBuffer.buffer, &offset);

	// WVP matrix
	glm::vec3 camPos {0.f, -2.f, -5.f};

	// World
	glm::mat4 world = glm::rotate(glm::mat4{1.f}, glm::radians(m_FrameNumber  * 0.4f), glm::vec3(0,1,0));
	// View
	glm::mat4 view = glm::translate(glm::mat4(1.f), camPos);
	// Projection
	glm::mat4 projection = glm::perspective(glm::radians(60.f), (float)m_WindowExtent.width / (float)m_WindowExtent.height, 0.1f, 200.f);
	projection[1][1] *= -1;

	glm::mat4 WVP = projection * view * world;

	MeshPushConstants constants;
	constants.wvp = WVP;

	vkCmdPushConstants(m_MainCommandBuffer, m_MeshPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(MeshPushConstants), &constants);

	// Draw the mesh
	vkCmdDraw(m_MainCommandBuffer, m_TeapotMesh.vertices.size(), 1, 0, 0);

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
		// Wait for the GPU to be done with the last frame
		vkDeviceWaitIdle(m_Device);

		m_MainDeletionQueue.Flush();

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

	VmaAllocatorCreateInfo allocatorInfo{};
	allocatorInfo.physicalDevice = m_PhysicalDevice;
	allocatorInfo.device = m_Device;
	allocatorInfo.instance = m_Instance;
	vmaCreateAllocator(&allocatorInfo, &m_Allocator);

	m_MainDeletionQueue.deletors.emplace_back([=]() {
		vmaDestroyAllocator(m_Allocator);
		});
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

	m_MainDeletionQueue.deletors.emplace_back([=]() {
			vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr); 
		});
}

void VulkanEngine::InitCommands()
{
	std::cout << "Initializing Commands\n";

	auto commandPoolInfo = vkinit::CommandPoolCreateInfo(m_GraphicsQueueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	VK_CHECK (vkCreateCommandPool(m_Device, &commandPoolInfo, nullptr, &m_CommandPool));

	auto commandBufferInfo = vkinit::CommandBufferAllocateInfo(m_CommandPool, 1);
	VK_CHECK(vkAllocateCommandBuffers(m_Device, &commandBufferInfo, &m_MainCommandBuffer));

	m_MainDeletionQueue.deletors.emplace_back([=]() {
		vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
		});
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

	m_MainDeletionQueue.deletors.emplace_back([=]() {
		vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);
		});
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

		m_MainDeletionQueue.deletors.emplace_back([=]() {
			vkDestroyFramebuffer(m_Device, m_Framebuffers[i], nullptr);
			vkDestroyImageView(m_Device, m_SwapChainImageViews[i], nullptr);
			});
	}
}

void VulkanEngine::InitializeSyncStructures()
{
	std::cout << "Initializing Sync Structures\n";

	VkFenceCreateInfo fenceCreateInfo{ vkinit::FenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT) };
	VK_CHECK(vkCreateFence(m_Device, &fenceCreateInfo, nullptr, &m_RenderFence));

	m_MainDeletionQueue.deletors.emplace_back([=]() {
		vkDestroyFence(m_Device, m_RenderFence, nullptr);
		});

	// Sempahore needs no flags
	VkSemaphoreCreateInfo semaphoreCreateInfo{ vkinit::SemaphoreCreateInfo() };

	VK_CHECK(vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_PresentSemaphore));
	VK_CHECK(vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_RenderSemaphore));

	m_MainDeletionQueue.deletors.emplace_back([=]()
		{
			vkDestroySemaphore(m_Device, m_PresentSemaphore, nullptr);
			vkDestroySemaphore(m_Device, m_RenderSemaphore, nullptr);
		});
}

bool VulkanEngine::LoadShaderModule(const char* filePath, VkShaderModule* outShaderModule)
{
	// Open the file stream, seek to the end of the file
	std::ifstream file(filePath, std::ios::ate | std::ios::binary);

	if (!file.is_open())
		return false;

	// Get the file size
	size_t fileSize = static_cast<size_t>(file.tellg());

	// SPRIV expects the buffer to be on uint32_t, so make sure to reserve a buffer big enough for that
	std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

	// Put cursor at the beginning of the file
	file.seekg(0);

	// Load the entire file into the buffer
	file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

	// Close the file stream
	file.close();

	// Create a new shader module, using the buffer we loaded
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.pNext = nullptr;

	// Code size has to be in bytes, so multiply the int in the buffer by the size of an int
	createInfo.codeSize = buffer.size() * sizeof(uint32_t);
	createInfo.pCode = buffer.data();

	// Validate creation
	VkShaderModule shaderModule;
	if (vkCreateShaderModule(m_Device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		return false;

	// Set the outShaderModule to the newly created shader module
	*outShaderModule = shaderModule;

	return true;
}

void VulkanEngine::InitializePipelines()
{
	std::cout << "Initializing Pipelines\n";

	VkShaderModule fragShader;
	if (!LoadShaderModule("../Graphics/Shaders/colored_triangle.frag.spv", &fragShader))
		std::cout << "Error building the triangle fragment shader module\n";
	else
		std::cout << "Successfully built the fragment shader module\n";

	VkShaderModule vertShader;
	if (!LoadShaderModule("../Graphics/Shaders/tri_mesh.vert.spv", &vertShader))
		std::cout << "Error building the triangle vertex shader module\n";
	else
		std::cout << "Successfully built the vertex shader module\n";

	// Build pipeline layout that controls inputs/outputs of the shader
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{ vkinit::PipelineLayoutCreateInfo() };

	// Setup push constants
	VkPushConstantRange pushConstant{};
	pushConstant.offset = 0;
	pushConstant.size = sizeof(MeshPushConstants);
	// Push constant is only available in the vertex shader
	pushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	pipelineLayoutInfo.pPushConstantRanges = &pushConstant;
	pipelineLayoutInfo.pushConstantRangeCount = 1;

	VK_CHECK(vkCreatePipelineLayout(m_Device, &pipelineLayoutInfo, nullptr, &m_MeshPipelineLayout));

	// Build the stage-create-info for both vertex and fragment stages.
	// This lets the pipeline know the shader modules per stage
	PipelineBuilder pipelineBuilder;

	pipelineBuilder.m_ShaderStages.push_back(
		vkinit::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, vertShader)
	);

	pipelineBuilder.m_ShaderStages.push_back(
		vkinit::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, fragShader)
	);

	// Vertex input controls how to read vertices from vertex buffers
	pipelineBuilder.m_VertexInputInfo = vkinit::VertexInputStateCreateInfo();

	VertexInputDescription vertexDesc = Vertex::GetVertexDescription();

	pipelineBuilder.m_VertexInputInfo.pVertexAttributeDescriptions = vertexDesc.attributes.data();
	pipelineBuilder.m_VertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexDesc.attributes.size());

	pipelineBuilder.m_VertexInputInfo.pVertexBindingDescriptions = vertexDesc.bindings.data();
	pipelineBuilder.m_VertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexDesc.bindings.size());

	// Set input assembly state, which controls primitive topology
	pipelineBuilder.m_InputAssembly = vkinit::InputAssemblyCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

	// Build the viewport and scissor from the swapchain extents
	pipelineBuilder.m_Viewport.x = 0.0f;
	pipelineBuilder.m_Viewport.y = 0.0f;
	pipelineBuilder.m_Viewport.width = static_cast<float>(m_WindowExtent.width);
	pipelineBuilder.m_Viewport.height = static_cast<float>(m_WindowExtent.height);
	pipelineBuilder.m_Viewport.minDepth = 0.0f;
	pipelineBuilder.m_Viewport.maxDepth = 1.0f;

	pipelineBuilder.m_Scissor.offset = { 0, 0 };
	pipelineBuilder.m_Scissor.extent = m_WindowExtent;

	// Build rasterizer
	pipelineBuilder.m_Rasterizer = vkinit::RasterizationStateCreateInfo(VK_POLYGON_MODE_FILL);

	// Build multisampling
	pipelineBuilder.m_Multisampling = vkinit::MultisamplingStateCreateInfo();

	// Build color blend attachment with no blending and writing to RGBA
	pipelineBuilder.m_ColorBlendAttachment = vkinit::ColorBlendAttachmentState();

	// Use the triangle layout
	pipelineBuilder.m_PipelineLayout = m_MeshPipelineLayout;

	// Build the pipeline
	m_MeshPipeline = pipelineBuilder.BuildPipeline(m_Device, m_RenderPass);

	vkDestroyShaderModule(m_Device, vertShader, nullptr);
	vkDestroyShaderModule(m_Device, fragShader, nullptr);

	m_MainDeletionQueue.deletors.emplace_back([=]() {
		vkDestroyPipeline(m_Device, m_MeshPipeline, nullptr);
		vkDestroyPipelineLayout(m_Device, m_MeshPipelineLayout, nullptr);
		});
}

void VulkanEngine::LoadMeshes()
{
	//make the array 3 vertices long
	m_TriangleMesh.vertices.resize(3);
				   
	// vertex positions
	m_TriangleMesh.vertices[0].position = { 1.f, 1.f, 0.0f };
	m_TriangleMesh.vertices[1].position = { -1.f, 1.f, 0.0f };
	m_TriangleMesh.vertices[2].position = { 0.f,-1.f, 0.0f };
				   
	// vertex colors, all green
	m_TriangleMesh.vertices[0].color = { 0.1f, 0.85f, 0.1f }; //pure green
	m_TriangleMesh.vertices[1].color = { 0.1f, 0.85f, 0.1f }; //pure green
	m_TriangleMesh.vertices[2].color = { 0.1f, 0.85f, 0.1f }; //pure green

	m_TeapotMesh.LoadFromObj("Meshes/Teapot.obj");

	UploadMesh(m_TeapotMesh);
}

void VulkanEngine::UploadMesh(Mesh& mesh)
{
	// Allocate vertex buffer
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.pNext = nullptr;

	// Total size, in bytes, of the buffer
	// Should hold all vertices
	bufferInfo.size = mesh.vertices.size() * sizeof(Vertex);

	// This buffer is going to be used as a Vertex Buffer
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

	// Let the buffer be shared between the CPU (write) and GPU (read) so that we can upload it
	VmaAllocationCreateInfo vmaallocInfo{};
	vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

	VK_CHECK(vmaCreateBuffer(m_Allocator, &bufferInfo, &vmaallocInfo,
		&mesh.vertexBuffer.buffer,
		&mesh.vertexBuffer.allocation,
		nullptr));

	// Add the destruction of buffer to the deletion queue
	m_MainDeletionQueue.deletors.emplace_back([=]() {
		vmaDestroyBuffer(m_Allocator, mesh.vertexBuffer.buffer, mesh.vertexBuffer.allocation);
		});

	// Copy vertex data into buffer
	void* data;
	vmaMapMemory(m_Allocator, mesh.vertexBuffer.allocation, &data);
	memcpy(data, mesh.vertices.data(), mesh.vertices.size() * sizeof(Vertex));
	vmaUnmapMemory(m_Allocator, mesh.vertexBuffer.allocation);
}