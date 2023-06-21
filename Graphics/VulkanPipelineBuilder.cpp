#include "VulkanPipelineBuilder.h"

#include <iostream>

using namespace leap::graphics;

VkPipeline PipelineBuilder::BuildPipeline(VkDevice device, VkRenderPass renderPass)
{
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.pNext = nullptr;

    // One viewport
    viewportState.viewportCount = 1;
    viewportState.pViewports = &m_Viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &m_Scissor;

    // Setup color blending. We are not using blending yet.
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.pNext = nullptr;

    // No blending
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &m_ColorBlendAttachment;

    // Build the pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = nullptr;

    // Pipeline shaders
    pipelineInfo.stageCount = static_cast<uint32_t>(m_ShaderStages.size());
    pipelineInfo.pStages = m_ShaderStages.data();
    
    // Vertex input
    pipelineInfo.pVertexInputState = &m_VertexInputInfo;

    // Input assembly
    pipelineInfo.pInputAssemblyState = &m_InputAssembly;

    // Viewport state
    pipelineInfo.pViewportState = &viewportState;

    // Rasterizer
    pipelineInfo.pRasterizationState = &m_Rasterizer;

    // Multisampling
    pipelineInfo.pMultisampleState = &m_Multisampling;

    // Color blending
    pipelineInfo.pColorBlendState = &colorBlending;

    // Pipeline layout
    pipelineInfo.layout = m_PipelineLayout;

    // Render pass
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;

    // Pipeline derivatives : None
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    VkPipeline pipeline;
    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS)
    {
        std::cout << "Error: failed to create graphics pipeline!\n";
        return VK_NULL_HANDLE;
    }
    else
        return pipeline;
}
