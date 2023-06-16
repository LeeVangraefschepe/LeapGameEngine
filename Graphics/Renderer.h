#pragma once
#include <memory>

class VulkanEngine;

class Renderer final
{
public:
	void Initialize();

private:
	std::unique_ptr<VulkanEngine> m_pVulkanEngine;
};