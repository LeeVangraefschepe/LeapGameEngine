#pragma once
#include <memory>
#include "VulkanEngine.h"

namespace leap::graphics
{
	class Renderer final
	{
	public:
		Renderer(class GLFWwindow* pWindow);

		void Initialize();
		void Draw();

	private:
		std::unique_ptr<VulkanEngine> m_pVulkanEngine;
	};
}
