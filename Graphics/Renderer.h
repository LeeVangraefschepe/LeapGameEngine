#pragma once
#include <memory>
#include "Vulkan/VulkanEngine.h"
#include "DirectX/DirectXEngine.h"

namespace leap::graphics
{
	class Renderer final
	{
	public:
		Renderer(class GLFWwindow* pWindow);

		void Initialize();
		void Draw();

	private:
		//Create interface for this later
		std::unique_ptr<DirectXEngine> m_pEngine;
	};
}
