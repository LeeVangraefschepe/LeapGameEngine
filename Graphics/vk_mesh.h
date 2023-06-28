#pragma once

#include <vector>
#include <vec3.hpp>

#include "VulkanTypes.h"

namespace leap::graphics
{
	struct VertexInputDescription
	{
		std::vector<VkVertexInputBindingDescription> bindings;
		std::vector<VkVertexInputAttributeDescription> attributes;

		VkPipelineVertexInputStateCreateFlags flags = 0;
	};

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;


		static VertexInputDescription GetVertexDescription();
	};

	struct Mesh
	{
		std::vector<Vertex> vertices;
		AllocatedBuffer vertexBuffer;

		bool LoadFromObj(const char* filename);
	};
}
