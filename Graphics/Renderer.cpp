#include "Renderer.h"
#include "VulkanEngine.h"

void Renderer::Initialize()
{
	m_pVulkanEngine = std::make_unique<VulkanEngine>();
	m_pVulkanEngine->Initialize();
}