#include "VulkanInitializers.h"

VkCommandPoolCreateInfo vkinit::CommandPoolCreateInfo(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags)
{
	// Create a command pool for commands submitted to the graphics queue
	VkCommandPoolCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	info.pNext = nullptr;

	// The command pool will be one that can submit graphics commands
	info.queueFamilyIndex = queueFamilyIndex;

	// We also want the pool to allow for resetting individual command buffers
	info.flags = flags;

	return info;
}

VkCommandBufferAllocateInfo vkinit::CommandBufferAllocateInfo(VkCommandPool pool, uint32_t count, VkCommandBufferLevel level)
{
	// Allocate the default command buffer that we will use for rendering
	VkCommandBufferAllocateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	info.pNext = nullptr;

	// Commands will be made from our m_CommandPool
	info.commandPool = pool;

	// Allocate just one command buffer
	info.commandBufferCount = count;

	// The command buffer is a primary buffer (that can submit to the queue)
	info.level = level;

	return info;
}

VkFenceCreateInfo vkinit::FenceCreateInfo(VkFenceCreateFlags flags)
{
	VkFenceCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = flags;
	return info;
}

VkSemaphoreCreateInfo vkinit::SemaphoreCreateInfo(VkSemaphoreCreateFlags flags)
{
	VkSemaphoreCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = flags;
	return info;
}

VkPipelineShaderStageCreateInfo vkinit::PipelineShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule shaderModule)
{
	VkPipelineShaderStageCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	info.pNext = nullptr;

	// Shader stage
	info.stage = stage;
	info.module = shaderModule;

	// Entry point in the shader
	info.pName = "main";

	return info;
}

VkPipelineVertexInputStateCreateInfo vkinit::VertexInputStateCreateInfo()
{
	VkPipelineVertexInputStateCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	info.pNext = nullptr;

	// No vertex bindings or attributes
	info.vertexBindingDescriptionCount = 0;
	info.vertexAttributeDescriptionCount = 0;

	return info;
}

VkPipelineInputAssemblyStateCreateInfo vkinit::InputAssemblyCreateInfo(VkPrimitiveTopology topology)
{
	VkPipelineInputAssemblyStateCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	info.pNext = nullptr;

	// Primitive topology
	info.topology = topology;
	info.primitiveRestartEnable = VK_FALSE;

	return info;
}

VkPipelineRasterizationStateCreateInfo vkinit::RasterizationStateCreateInfo(VkPolygonMode polygonMode)
{
	VkPipelineRasterizationStateCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	info.pNext = nullptr;

	// How to handle polygons
	info.depthClampEnable = VK_FALSE;
	info.rasterizerDiscardEnable = VK_FALSE;

	// How to handle filling points between vertices
	info.polygonMode = polygonMode; // Solid / Wireframe
	info.lineWidth = 1.0f;

	// No backface culling
	info.cullMode = VK_CULL_MODE_NONE;
	info.frontFace = VK_FRONT_FACE_CLOCKWISE;

	// No depth bias
	info.depthBiasEnable = VK_FALSE;
	info.depthBiasConstantFactor = 0.0f;
	info.depthBiasClamp = 0.0f;
	info.depthBiasSlopeFactor = 0.0f;

	return info;
}

VkPipelineMultisampleStateCreateInfo vkinit::MultisamplingStateCreateInfo()
{
	VkPipelineMultisampleStateCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	info.pNext = nullptr;

	// No multisampling
	info.sampleShadingEnable = VK_FALSE;
	// 1 sample per pixel
	info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	info.minSampleShading = 1.0f;
	info.pSampleMask = nullptr;
	info.alphaToCoverageEnable = VK_FALSE;
	info.alphaToOneEnable = VK_FALSE;

	return info;
}

VkPipelineColorBlendAttachmentState vkinit::ColorBlendAttachmentState()
{
	VkPipelineColorBlendAttachmentState info{};
	info.colorWriteMask = 
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	info.blendEnable = VK_FALSE;

	return info;
}

VkPipelineLayoutCreateInfo vkinit::PipelineLayoutCreateInfo()
{
	VkPipelineLayoutCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	info.pNext = nullptr;

	// Empty defaults
	info.flags = 0;
	info.setLayoutCount = 0;
	info.pSetLayouts = nullptr;
	info.pushConstantRangeCount = 0;
	info.pPushConstantRanges = nullptr;

	return info;
}
