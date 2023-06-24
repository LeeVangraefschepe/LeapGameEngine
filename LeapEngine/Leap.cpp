#include "Leap.h"

#include <chrono>
#include <functional>

#include "InputManager.h"
#include "Renderer.h"
#include "ServiceLocator/ServiceLocator.h"
#include "Systems/FmodAudioSystem.h"

#include <iostream>
#include <glfw3.h>

#include "GameContext/GameContext.h"
#include "SceneGraph/SceneManager.h"

leap::LeapEngine::LeapEngine()
{
    std::cout << "Engine created\n";

    /* Initialize the library */
    if (!glfwInit())
        return;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_pWindow = glfwCreateWindow(1280, 720, "Leap Engine", nullptr, nullptr);
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

    input::InputManager::GetInstance().SetWindow(m_pWindow);
}

void leap::LeapEngine::Run(int desiredFPS)
{
	std::cout << "Engine startup\n";

    m_pRenderer = std::make_unique<graphics::Renderer>(m_pWindow);
    m_pRenderer->Initialize();

    ServiceLocator::RegisterAudioSystem<audio::FmodAudioSystem>();

    auto& input = input::InputManager::GetInstance();
    auto& gameContext = GameContext::GetInstance();
    auto& audio = ServiceLocator::GetAudio();

    auto& sceneManager = SceneManager::GetInstance();
    sceneManager.LoadScene(0);

    const float frameTimeMs{ static_cast<float>(100) / static_cast<float>(desiredFPS) };

    while (!glfwWindowShouldClose(m_pWindow))
    {
        const auto currentTime = std::chrono::high_resolution_clock::now();

        //Update gamecontext (Timer class)
        gameContext.Update();

        sceneManager.OnFrameStart();
        sceneManager.Update();

        audio.Update();

        glfwPollEvents();
        input.ProcessInput();

        sceneManager.LateUpdate();

        //Render here
        glClearColor(0.2f, 0.7f, 0.5f, 1.0f);
        sceneManager.Render();
        m_pRenderer->Draw();
        sceneManager.OnGUI();
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(m_pWindow);

        sceneManager.OnFrameEnd();

        //Sleep to sync back with desired fps
        const auto sleepTimeMs = frameTimeMs - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - currentTime).count();
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepTimeMs)));
    }

    glfwTerminate();
}