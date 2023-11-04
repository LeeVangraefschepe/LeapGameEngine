#include "Leap.h"

#include <chrono>
#include "InputManager.h"
#include "Interfaces/IRenderer.h"
#include "ServiceLocator/ServiceLocator.h"
#include "FMOD/FmodAudioSystem.h"
#include "DirectX/DirectXEngine.h"

#include "Debug.h"
#include <glfw3.h>

#include "GameContext/GameContext.h"
#include "GameContext/Timer.h"
#include "GameContext/Window.h"
#include "SceneGraph/SceneManager.h"

leap::LeapEngine::LeapEngine(int width, int height, const std::string& title)
{
    /* Initialize the library */
    if (!glfwInit())
        Debug::LogError("LeapEngine Error: GLFW initialisation error");

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_pWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_pWindow)
    {
        glfwTerminate();
        Debug::LogError("LeapEngine Error: Failed to create window");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_pWindow);
    Debug::Log("LeapEngine Log: Window created successfully");

    Debug::Log("LeapEngine Log: Linking window to the Input library");
    input::InputManager::GetInstance().SetWindow(m_pWindow);

    Debug::Log("LeapEngine Log: Linking window to the game context");
    GameContext::GetInstance().CreateWindowWrapper(m_pWindow);

    Debug::Log("LeapEngine Log: Registering default audio system (FMOD)");
    ServiceLocator::RegisterAudioSystem<audio::FmodAudioSystem>();

    Debug::Log("LeapEngine Log: Registering default renderer (DirectX)");
    ServiceLocator::RegisterRenderer<graphics::DirectXEngine>(m_pWindow);

    m_pRenderer = &ServiceLocator::GetRenderer();
    m_pRenderer->Initialize();

    // Set default icon
    Debug::Log("LeapEngine Log: Setting default window icon");
    GameContext::GetInstance().GetWindow()->SetIcon("Data/Engine/Images/logo.png");

    Debug::Log("LeapEngine Log: Engine is successfully constructed");
}

leap::LeapEngine::~LeapEngine()
{
    Debug::Log("LeapEngine Log: Engine destroyed");
}

void leap::LeapEngine::Run(int desiredFPS)
{
    auto& input = input::InputManager::GetInstance();
    auto& gameContext = GameContext::GetInstance();

    Debug::Log("LeapEngine Log: Loading default scene");
    auto& sceneManager = SceneManager::GetInstance();
    const auto timer = gameContext.GetTimer();
    sceneManager.LoadScene(0);

    const float frameTimeMs{ static_cast<float>(100) / static_cast<float>(desiredFPS) };
    float fixedTotalTime{};

    auto& audio = ServiceLocator::GetAudio();

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
        gameContext.OnGUI();

        m_pRenderer->Draw();
        glfwSwapBuffers(m_pWindow);

        sceneManager.OnFrameEnd();

        //Sleep to sync back with desired fps
        const auto sleepTimeMs = frameTimeMs - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - currentTime).count();
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepTimeMs)));
    }

    sceneManager.UnloadScene();

    Debug::Log("LeapEngine Log: Destroying window");
    glfwTerminate();
}