#include "Renderer.h"

using namespace leap::graphics;

Renderer::Renderer(GLFWwindow* pWindow)
	: m_pEngine{ std::make_unique<DirectXEngine>(pWindow) }
{}

void Renderer::Initialize()
{
	m_pEngine->Initialize();
}

void Renderer::Draw()
{
	m_pEngine->Draw();
}