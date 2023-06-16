#pragma once
#include <memory>


class Renderer final
{
public:
	Renderer(class GLFWwindow* pWindow);
	~Renderer();

	void Initialize();
	void Draw();
	void Run();
	void Cleanup();

private:
	class VulkanEngine* m_pVulkanEngine;
};