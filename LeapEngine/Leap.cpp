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
    std::cout << "LeapEngine Log: Engine created\n";

    /* Initialize the library */
    if (!glfwInit())
        throw std::runtime_error("LeapEngine Error: GLFW initialisation error");

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_pWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_pWindow)
    {
        glfwTerminate();
        throw std::runtime_error("LeapEngine Error: Failed to create window");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_pWindow);

    std::cout << "LeapEngine Log: Window created successfully\n";
}

leap::LeapEngine::LeapEngine(int width, int height, const std::string& title, const std::string&) : LeapEngine(width, height, title)
{
}

leap::LeapEngine::~LeapEngine()
{
    std::cout << "LeapEngine Log: Engine destroyed\n";
}

void leap::LeapEngine::Run(int desiredFPS)
{
    auto& input = input::InputManager::GetInstance();
    auto& gameContext = GameContext::GetInstance();
    auto& audio = ServiceLocator::GetAudio();

    std::cout << "LeapEngine Log: Linking window to the Input library\n";
    input::InputManager::GetInstance().SetWindow(m_pWindow);

    std::cout << "LeapEngine Log: Linking window to the game context\n";
    gameContext.CreateWindowWrapper(m_pWindow);

    std::cout << "LeapEngine Log: Registering default audio system (FMOD)\n";
    ServiceLocator::RegisterAudioSystem<audio::FmodAudioSystem>();

    std::cout << "LeapEngine Log: Registering default renderer (DirectX)\n";
    ServiceLocator::RegisterRenderer<graphics::DirectXEngine>(m_pWindow);

    m_pRenderer = &ServiceLocator::GetRenderer();
    m_pRenderer->Initialize();

    std::cout << "LeapEngine Log: Loading default scene\n";
    auto& sceneManager = SceneManager::GetInstance();
    const auto timer = gameContext.GetTimer();
    sceneManager.LoadScene(0);

    const float frameTimeMs{ static_cast<float>(100) / static_cast<float>(desiredFPS) };
    float fixedTotalTime{};

    while (!glfwWindowShouldClose(m_pWindow))
    {
        const auto currentTime = std::chrono::high_resolution_clock::now();

        // Update gamecontext (Timer & window)
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

        m_pRenderer->GuiDraw();
        sceneManager.OnGUI();

        m_pRenderer->Draw();
        glfwSwapBuffers(m_pWindow);

        sceneManager.OnFrameEnd();

        //Sleep to sync back with desired fps
        const auto sleepTimeMs = frameTimeMs - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - currentTime).count();
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepTimeMs)));
    }

    std::cout << "LeapEngine Log: Destroying window\n";
    glfwTerminate();
}