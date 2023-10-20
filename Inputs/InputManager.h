#pragma once
#include "Singleton.h"

#include <vec2.hpp>
#include <vector>
#include <map>
#include <memory>

class GLFWwindow;

namespace leap
{
    class Command;
}

namespace leap::input
{
    class Mouse;
    class Keyboard;
	class InputManager final : public Singleton<InputManager>
	{
	public:
#pragma region InputEnums
        enum InputType
        {
            EventRelease,
            EventPress,
            EventRepeat
        };
#pragma endregion
        void SetWindow(GLFWwindow* window);
        bool ProcessInput() const;

        Mouse* GetMouse() const { return m_pMouse.get(); }
        Keyboard* GetKeyboard() const { return m_pKeyboard.get(); }

        InputManager(const InputManager& other) = delete;
        InputManager(InputManager&& other) = delete;
        InputManager& operator=(const InputManager& other) = delete;
        InputManager& operator=(InputManager&& other) = delete;
        virtual ~InputManager();
	private:
        InputManager();
        friend Singleton;

        GLFWwindow* m_pWindow {nullptr};
        std::unique_ptr<Mouse> m_pMouse;
        std::unique_ptr<Keyboard> m_pKeyboard;
	};
}
