#include "Leap.h"

#include <chrono>
#include "InputManager.h"
#include "Interfaces/IRenderer.h"
#include "ServiceLocator/ServiceLocator.h"
#include "FMOD/FmodAudioSystem.h"
#include "DirectX/DirectXEngine.h"
#include "PhysX/PhysXEngine.h"

#include "Debug.h"
#include <glfw3.h>

#include "GameContext/GameContext.h"
#include "GameContext/Timer.h"
#include "GameContext/Window.h"
#include "SceneGraph/SceneManager.h"

#include "Physics/PhysicsSync.h"

leap::LeapEngine::LeapEngine(int width, int height, const char* title)
{
    /* Initialize the library */
    if (!glfwInit())
        Debug::LogError("LeapEngine Error: GLFW initialisation error");

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_pWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
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

    Debug::Log("LeapEngine Log: Registering default physics (PhysX)");
    ServiceLocator::RegisterPhysics<physics::PhysXEngine>();

    Debug::Log("LeapEngine Log: Engine is successfully constructed");
}

leap::LeapEngine::~LeapEngine()
{
    Debug::Log("LeapEngine Log: Engine destroyed");
}

void leap::LeapEngine::Run(const std::function<void()>& afterInitialize, int desiredFPS)
{
    auto& renderer{ ServiceLocator::GetRenderer() };
    renderer.Initialize();

    afterInitialize();

    auto& input{ input::InputManager::GetInstance() };
    auto& gameContext{ GameContext::GetInstance() };

    Debug::Log("LeapEngine Log: Loading default scene");
    auto& sceneManager = SceneManager::GetInstance();
    const auto timer = gameContext.GetTimer();
    sceneManager.LoadScene(0);

    const int frameTimeNs{ 1'000'000'000 / desiredFPS };
    float fixedTotalTime{};

    auto& audio{ ServiceLocator::GetAudio() };
    auto& physics{ ServiceLocator::GetPhysics() };
    physics.SetSyncFunc(PhysicsSync::SetTransform, PhysicsSync::GetTransform);
    physics.OnCollisionEnter().AddListener(PhysicsSync::OnCollisionEnter);
    physics.OnCollisionStay().AddListener(PhysicsSync::OnCollisionStay);
    physics.OnCollisionExit().AddListener(PhysicsSync::OnCollisionExit);
    physics.OnTriggerEnter().AddListener(PhysicsSync::OnTriggerEnter);
    physics.OnTriggerStay().AddListener(PhysicsSync::OnTriggerStay);
    physics.OnTriggerExit().AddListener(PhysicsSync::OnTriggerExit);
    

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
            physics.Update(fixedInterval);
        }

        sceneManager.Update();

        audio.Update();

        sceneManager.LateUpdate();

        renderer.GuiDraw();
        sceneManager.OnGUI();
        gameContext.OnGUI();

        renderer.DrawLines(physics.GetDebugDrawings());
        renderer.Draw();
        glfwSwapBuffers(m_pWindow);

        sceneManager.OnFrameEnd();

        // Wait to sync back with desired fps
        long long curFrameTimeNs{};
        do
        {
            curFrameTimeNs = (std::chrono::high_resolution_clock::now() - currentTime).count();
        } while (frameTimeNs - curFrameTimeNs > 0);
    }

    sceneManager.UnloadScene();

    Debug::Log("LeapEngine Log: Destroying window");
    glfwTerminate();
}