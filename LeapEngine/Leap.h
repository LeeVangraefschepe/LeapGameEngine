#pragma once
#include <memory>
#include <string>

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
		LeapEngine(int width, int height, const std::string& title);
		LeapEngine(int width, int height, const std::string& title, const std::string& iconPath);
		~LeapEngine();
		LeapEngine(const LeapEngine& other) = delete;
		LeapEngine(LeapEngine&& other) = delete;
		LeapEngine& operator=(const LeapEngine& other) = delete;
		LeapEngine& operator=(LeapEngine&& other) = delete;

		void Run(int desiredFPS);

	private:
		GLFWwindow* m_pWindow{};
		leap::graphics::IRenderer* m_pRenderer{};
	};
}