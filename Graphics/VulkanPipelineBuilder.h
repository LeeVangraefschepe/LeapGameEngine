#pragma once
#include <vector>
#include "VulkanTypes.h"

namespace leap::graphics
{
	class PipelineBuilder final
	{
	public:
		std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
		VkPipelineVertexInputStateCreateInfo m_VertexInputInfo{};
		VkPipelineInputAssemblyStateCreateInfo m_InputAssembly{};
		VkViewport m_Viewport{};
		VkRect2D m_Scissor{};
		VkPipelineRasterizationStateCreateInfo m_Rasterizer{};
		VkPipelineColorBlendAttachmentState m_ColorBlendAttachment{};
		VkPipelineMultisampleStateCreateInfo m_Multisampling{};
		VkPipelineLayout m_PipelineLayout{ VK_NULL_HANDLE };
		VkPipelineDepthStencilStateCreateInfo m_DepthStencil;

		VkPipeline BuildPipeline(VkDevice device, VkRenderPass renderPass);
	};
}