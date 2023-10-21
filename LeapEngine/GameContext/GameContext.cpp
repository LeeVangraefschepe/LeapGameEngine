#include "GameContext.h"
#include "Window.h"
#include "Timer.h"
#include "Logger/ILogger.h"

leap::GameContext::~GameContext()
{
}

leap::GameContext::GameContext()
{
	m_pTimer = std::make_unique<Timer>();
}

void leap::GameContext::Update()
{
	m_pTimer->Update();
	m_pWindow->Update();

	for (const auto& logger : m_pLoggers)
	{
		logger->OnUpdate();
	}
}

void leap::GameContext::OnGUI()
{
	for (const auto& logger : m_pLoggers)
	{
		logger->OnGUI();
	}
}

void leap::GameContext::CreateWindowWrapper(GLFWwindow* window)
{
	m_pWindow = std::make_unique<Window>(window);
}