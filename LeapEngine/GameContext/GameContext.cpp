#include "GameContext.h"

#include "ConsoleLogger.h"
#include "Window.h"
#include "ImGuiLogger.h"
#include "Timer.h"

leap::GameContext::~GameContext()
{
}

leap::GameContext::GameContext()
{
	m_pTimer = std::make_unique<Timer>();
	m_pImguiLogger = std::make_unique<ImGuiLogger>();
	m_pConsoleLogger = std::make_unique<ConsoleLogger>();
}

void leap::GameContext::Update()
{
	m_pTimer->Update();
	m_pWindow->Update();
}

void leap::GameContext::OnGUI()
{
	if (m_pImguiLogger) m_pImguiLogger->OnGUI();
}

void leap::GameContext::CreateWindowWrapper(GLFWwindow* window)
{
	m_pWindow = std::make_unique<Window>(window);
}