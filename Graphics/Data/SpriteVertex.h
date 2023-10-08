#pragma once

#include "vec4.hpp"
#include "vec3.hpp"
#include "vec2.hpp"

namespace leap::graphics
{
	struct SpriteVertex final
	{
		glm::vec3 position{};
		glm::vec2 pivot{ 0.5f, 0.5f };
		glm::vec2 size{ 1.0f, 1.0f };
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
	};
}