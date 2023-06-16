#pragma once

class GLFWwindow;
class Renderer;

namespace leap
{
	class LeapEngine final
	{
	public:
		LeapEngine();
		~LeapEngine();

		void Run();
	private:

		GLFWwindow* m_pWindow;
		Renderer* m_pRenderer;
	};
}