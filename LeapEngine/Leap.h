#pragma once
#include <functional>
#include <memory>

class GLFWwindow;

namespace leap
{
	namespace graphics
	{
		class Renderer;
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

		void Run(const std::function<void()>& load, int desiredFPS);

	private:
		GLFWwindow* m_pWindow{};
		std::unique_ptr<leap::graphics::Renderer> m_pRenderer;
	};
}