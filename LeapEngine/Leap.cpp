#include "Leap.h"

#include <chrono>
#include "InputManager.h"
#include "Interfaces/IRenderer.h"
#include "ServiceLocator/ServiceLocator.h"
#include "Systems/FmodAudioSystem.h"
#include "DirectX/DirectXEngine.h"

#include "vec3.hpp"

#include <iostream>
#include <glfw3.h>

#include "GameContext/GameContext.h"
#include "SceneGraph/SceneManager.h"

leap::LeapEngine::LeapEngine(int width, int height, const std::string& title)
{
    std::cout << "Engine created\n";

    /* Initialize the library */
    if (!glfwInit())
        return;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_pWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
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

leap::LeapEngine::LeapEngine(int width, int height, const std::string& title, const std::string& iconPath) : LeapEngine(width, height, title)
{
    iconPath;
}

void leap::LeapEngine::Run(int desiredFPS)
{
	std::cout << "Engine startup\n";

    auto& input = input::InputManager::GetInstance();
    auto& gameContext = GameContext::GetInstance();
    auto& audio = ServiceLocator::GetAudio();

    gameContext.CreateWindowWrapper(m_pWindow);

    ServiceLocator::RegisterAudioSystem<audio::FmodAudioSystem>();
    ServiceLocator::RegisterRenderer<graphics::DirectXEngine>(m_pWindow);

    m_pRenderer = &ServiceLocator::GetRenderer();
    m_pRenderer->Initialize();

    auto& sceneManager = SceneManager::GetInstance();
    const auto timer = gameContext.GetTimer();
    sceneManager.LoadScene(0);

    const float frameTimeMs{ static_cast<float>(100) / static_cast<float>(desiredFPS) };
    float fixedTotalTime{};

    while (!glfwWindowShouldClose(m_pWindow))
    {
        const auto currentTime = std::chrono::high_resolution_clock::now();

        //Update gamecontext (Timer class)
        gameContext.Update();
        fixedTotalTime += timer->GetDeltaTime();

        glfwPollEvents();
        input.ProcessInput();

        sceneManager.OnFrameStart();

        const float fixedInterval = timer->GetFixedTime();
        while (fixedTotalTime >= fixedInterval)
        {
            fixedTotalTime -= fixedInterval;
            sceneManager.FixedUpdate();
        }

        sceneManager.Update();

        audio.Update();

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