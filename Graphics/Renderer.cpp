#include "Renderer.h"

using namespace leap::graphics;

Renderer::Renderer(GLFWwindow*)
	: m_pEngine{ std::make_unique<DirectXEngine>() }
{}

void Renderer::Initialize()
{
	m_pEngine->Initialize();
}

void Renderer::Draw()
{
	m_pEngine->Draw();
}