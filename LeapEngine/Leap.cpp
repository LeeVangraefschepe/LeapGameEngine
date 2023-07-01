#include "Leap.h"

#include <chrono>
#include <functional>

#include "InputManager.h"
#include "Renderer.h"
#include "ServiceLocator/ServiceLocator.h"
#include "Systems/FmodAudioSystem.h"

#include "vec3.hpp"

#include <iostream>
#include <glfw3.h>

#include "GameContext/GameContext.h"

#include "SceneGraph/GameObject.h"
#include "Components/Transform/Transform.h"

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

leap::LeapEngine::~LeapEngine()
{

}

void leap::LeapEngine::Run(const std::function<void()>& load, int desiredFPS)
{
	std::cout << "Engine startup\n";
    load();

    m_pRenderer = std::make_unique<graphics::Renderer>(m_pWindow);
    m_pRenderer->Initialize();

    ServiceLocator::RegisterAudioSystem<audio::FmodAudioSystem>();

    auto& input = input::InputManager::GetInstance();
    auto& gameContext = GameContext::GetInstance();
    auto& audio = ServiceLocator::GetAudio();

    auto rootObj{ std::make_unique<GameObject>("Root") };
    auto child{ rootObj->CreateChild("Child") };
    child->GetTransform()->Translate(20.0f, 5.0f, -10.0f);
    child->GetTransform()->Rotate(90.0f, 0.0f, 0.0f);
    child->GetTransform()->Scale(0.6f, 0.6f, 0.6f);
    auto grandChild{ child->CreateChild("GrandChild") };
    grandChild->GetTransform()->Translate(1.0f, -1.0f,5.0f);
    grandChild->GetTransform()->Rotate(0.0f, 90.0f, 0);
    grandChild->GetTransform()->Scale(2.0f, 2.0f, 2.0f);
    auto grandGrandChild{ grandChild->CreateChild("GrandGrandChild") };
    grandGrandChild->GetTransform()->Translate(1.0f, -1.0f, 5.0f);
    grandGrandChild->GetTransform()->Rotate(0.0f, -45.0f, -20.0f);
    grandGrandChild->GetTransform()->Scale(2.0f, 2.0f, 2.0f);

    const float frameTimeMs{ static_cast<float>(100) / static_cast<float>(desiredFPS) };

    while (!glfwWindowShouldClose(m_pWindow))
    {
        const auto currentTime = std::chrono::high_resolution_clock::now();

        //Update gamecontext (Timer class)
        gameContext.Update();

        //Update audio system
        audio.Update();

        //Poll for and process events
        glfwPollEvents();
        input.ProcessInput();

        rootObj->OnFrameStart();

        const auto& childPosition{ child->GetTransform()->GetWorldPosition() };
        const auto& childRotation{ child->GetTransform()->GetWorldRotation() };
        const auto& childEuler{ child->GetTransform()->GetWorldEulerDegrees() };
        const auto& childScale{ child->GetTransform()->GetWorldScale() };
        const auto& gChildPosition{ grandChild->GetTransform()->GetWorldPosition() };
        const auto& gChildRotation{ grandChild->GetTransform()->GetWorldRotation() };
        const auto& gChildEuler{ grandChild->GetTransform()->GetWorldEulerDegrees() };
        const auto& gChildScale{ grandChild->GetTransform()->GetWorldScale() };
        const auto& ggChildPosition{ grandGrandChild->GetTransform()->GetWorldPosition() };
        const auto& ggChildRotation{ grandGrandChild->GetTransform()->GetWorldRotation() };
        const auto& ggChildEuler{ grandGrandChild->GetTransform()->GetWorldEulerDegrees() };
        const auto& ggChildScale{ grandGrandChild->GetTransform()->GetWorldScale() };

        std::cout << "child " << childPosition.x << ", " << childPosition.y << ", " << childPosition.z << "\n";
        std::cout << "child " << childRotation.x << ", " << childRotation.y << ", " << childRotation.z << ", " << childRotation.w << "\n";
        std::cout << "child " << childEuler.x << ", " << childEuler.y << ", " << childEuler.z << "\n";
        std::cout << "child " << childScale.x << ", " << childScale.y << ", " << childScale.z << "\n";
        std::cout << "grandchild " << gChildPosition.x << ", " << gChildPosition.y << ", " << gChildPosition.z << "\n";
        std::cout << "grandchild " << gChildRotation.x << ", " << gChildRotation.y << ", " << gChildRotation.z << ", " << gChildRotation.w << "\n";
        std::cout << "grandchild " << gChildEuler.x << ", " << gChildEuler.y << ", " << gChildEuler.z << "\n";
        std::cout << "grandchild " << gChildScale.x << ", " << gChildScale.y << ", " << gChildScale.z << "\n";
        std::cout << "grandgrandchild " << ggChildPosition.x << ", " << ggChildPosition.y << ", " << ggChildPosition.z << "\n";
        std::cout << "grandgrandchild " << ggChildRotation.x << ", " << ggChildRotation.y << ", " << ggChildRotation.z << ", " << ggChildRotation.w << "\n";
        std::cout << "grandgrandchild " << ggChildEuler.x << ", " << ggChildEuler.y << ", " << ggChildEuler.z << "\n";
        std::cout << "grandgrandchild " << ggChildScale.x << ", " << ggChildScale.y << ", " << ggChildScale.z << "\n";

        //Render here
        glClearColor(0.2f, 0.7f, 0.5f, 1.0f);
        //m_pRenderer->Draw();
        glClear(GL_COLOR_BUFFER_BIT);

        //Swap front and back buffers
        glfwSwapBuffers(m_pWindow);

        //Sleep to sync back with desired fps
        const auto sleepTimeMs = frameTimeMs - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - currentTime).count();
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepTimeMs)));
    }

    glfwTerminate();
}