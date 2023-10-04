#include "GameContext.h"

leap::GameContext::~GameContext()
{
}

void leap::GameContext::Update()
{
	m_pTimer->Update();
	m_pWindow->Update();
}

void leap::GameContext::CreateWindowWrapper(GLFWwindow* window)
{
	m_pWindow = std::make_unique<Window>(window);
}