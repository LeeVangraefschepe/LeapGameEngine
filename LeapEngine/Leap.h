#pragma once
#include <memory>

class GLFWwindow;

namespace leap
{
	namespace graphics
	{
		class IRenderer;
	}

	class LeapEngine final
	{
	public:
		LeapEngine();
		~LeapEngine();
		LeapEngine(const LeapEngine& other) = delete;
		LeapEngine(LeapEngine&& other) = delete;
		LeapEngine& operator=(const LeapEngine& other) = delete;
		LeapEngine& operator=(LeapEngine&& other) = delete;

		void Run(int desiredFPS);

	private:
		GLFWwindow* m_pWindow{};
		leap::graphics::IRenderer* m_pRenderer;
	};
}