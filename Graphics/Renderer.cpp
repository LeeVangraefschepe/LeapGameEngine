#include "Renderer.h"
#include "VulkanEngine.h"

Renderer::Renderer(GLFWwindow* pWindow)
	: m_pVulkanEngine{ new VulkanEngine(pWindow) }
{}

Renderer::~Renderer()
{
	Cleanup();
}

void Renderer::Initialize()
{
	m_pVulkanEngine->Initialize();
}

void Renderer::Draw()
{
	m_pVulkanEngine->Draw();
}

void Renderer::Run()
{
	m_pVulkanEngine->Run();
}

void Renderer::Cleanup()
{
	m_pVulkanEngine->Cleanup();
	delete m_pVulkanEngine;
}