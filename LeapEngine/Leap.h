#pragma once

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
		explicit LeapEngine(int width, int height, const char* title);
		~LeapEngine();
		LeapEngine(const LeapEngine& other) = delete;
		LeapEngine(LeapEngine&& other) = delete;
		LeapEngine& operator=(const LeapEngine& other) = delete;
		LeapEngine& operator=(LeapEngine&& other) = delete;

		void Run(int desiredFPS);

	private:
		GLFWwindow* m_pWindow{};
	};
}