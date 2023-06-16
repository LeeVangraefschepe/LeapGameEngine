#pragma once

class GLFWwindow;
class VulkanEngine final
{
public:
	VulkanEngine(GLFWwindow* pWindow);

	void Initialize();
	void Draw();
	void Cleanup();

private:
	GLFWwindow* m_pWindow{ nullptr };
	bool m_IsInitialized{ false };
	int m_FrameNumber{ 0 };
};