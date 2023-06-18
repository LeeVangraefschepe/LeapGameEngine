#include "Renderer.h"

Renderer::Renderer(GLFWwindow* pWindow)
	: m_pVulkanEngine{ std::make_unique<VulkanEngine>(pWindow) }
{}

void Renderer::Initialize()
{
	m_pVulkanEngine->Initialize();
}

void Renderer::Draw()
{
	m_pVulkanEngine->Draw();
}