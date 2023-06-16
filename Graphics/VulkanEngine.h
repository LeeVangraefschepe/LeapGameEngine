#pragma once

class VulkanEngine final
{
public:
	bool m_IsInitialized{ false };
	int m_FrameNumber{ 0 };

	class GLFWwindow* m_pWindow{ nullptr };

	void Initialize();
	void Cleanup();
	void Draw();
	void Run();
};
