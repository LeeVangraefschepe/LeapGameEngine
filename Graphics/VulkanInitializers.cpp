#include "VulkanInitializers.h"

VkCommandPoolCreateInfo vkinit::CommandPoolCreateInfo(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags)
{
	// Create a command pool for commands submitted to the graphics queue
	VkCommandPoolCreateInfo commandPoolInfo{};
	commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolInfo.pNext = nullptr;

	// The command pool will be one that can submit graphics commands
	commandPoolInfo.queueFamilyIndex = queueFamilyIndex;

	// We also want the pool to allow for resetting individual command buffers
	commandPoolInfo.flags = flags;

	return commandPoolInfo;
}

VkCommandBufferAllocateInfo vkinit::CommandBufferAllocateInfo(VkCommandPool pool, uint32_t count, VkCommandBufferLevel level)
{
	// Allocate the default command buffer that we will use for rendering
	VkCommandBufferAllocateInfo commandBufferInfo{};
	commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferInfo.pNext = nullptr;

	// Commands will be made from our m_CommandPool
	commandBufferInfo.commandPool = pool;

	// Allocate just one command buffer
	commandBufferInfo.commandBufferCount = count;

	// The command buffer is a primary buffer (that can submit to the queue)
	commandBufferInfo.level = level;

	return commandBufferInfo;
}
