#pragma once

class GLFWwindow;

namespace leap
{
	class LeapEngine final
	{
	public:
		LeapEngine();
		~LeapEngine() = default;

		void Run();
	private:
		GLFWwindow* m_pWindow;
	};
}