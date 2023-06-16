#include "VulkanEngine.h"

#include <glfw3.h>

#include "VulkanTypes.h"
#include "VulkanInitializers.h"

VulkanEngine::VulkanEngine(GLFWwindow* pWindow)
	: m_pWindow{ pWindow }
{}

void VulkanEngine::Initialize()
{
	m_IsInitialized = true;
}

void VulkanEngine::Draw()
{
	
}

void VulkanEngine::Cleanup()
{

}