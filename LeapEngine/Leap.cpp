#include "Leap.h"

#include "InputManager.h"
#include "Renderer.h"

#include <iostream>
#include <glfw3.h>


leap::LeapEngine::LeapEngine()
{
    std::cout << "Engine created\n";

    /* Initialize the library */
    if (!glfwInit())
        return;

    /* Create a windowed mode window and its OpenGL context */
    m_pWindow = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!m_pWindow)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }

    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize glfw");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_pWindow);

    InputManager::GetInstance().SetWindow(m_pWindow);
}

leap::LeapEngine::~LeapEngine()
{
    delete m_pRenderer;
}

void leap::LeapEngine::Run()
{
	std::cout << "Engine startup\n";

    m_pRenderer = new Renderer(m_pWindow);
    m_pRenderer->Initialize();

    auto& input = InputManager::GetInstance();
    input.AddCommand(std::make_shared<DebugCommandInput>(), InputManager::EventPress, InputManager::KeyboardInput::KeyQ);
    input.AddCommand(std::make_shared<DebugCommandInput>(), InputManager::EventPress, InputManager::MouseInput::LeftButton);
    input.AddCommand(std::make_shared<DebugCommandInput>(), InputManager::WheelInput::DownWheel);

    while (!glfwWindowShouldClose(m_pWindow))
    {
        /* Poll for and process events */
        glfwPollEvents();
        input.ProcessInput();

        /* Render here */
        glClearColor(0.2f, 0.7f, 0.5f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(m_pWindow);
    }

    glfwTerminate();
}
