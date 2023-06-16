#pragma once

class Renderer final
{
public:
	Renderer(class GLFWwindow* pWindow);
	~Renderer();

	void Initialize();
	void Draw();
	void Cleanup();

private:
	class VulkanEngine* m_pVulkanEngine;
};