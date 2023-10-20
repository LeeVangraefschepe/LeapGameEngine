#pragma once

#include "vec4.hpp"
#include "vec3.hpp"
#include "vec2.hpp"

namespace leap::graphics
{
	struct Vertex final
	{
		glm::vec3 position{};
		glm::vec3 normal{};
		glm::vec2 uv{};
		glm::vec3 tangent{};
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec3 viewDirection{};
	};
}