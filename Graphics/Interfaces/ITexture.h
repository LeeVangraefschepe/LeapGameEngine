#pragma once

#include "vec2.hpp"
#include "vec4.hpp"

namespace leap::graphics
{
	class ITexture
	{
	public:
		virtual ~ITexture() = default;

		virtual glm::vec4 GetPixel(int x, int y) const = 0;
		virtual glm::ivec2 GetSize() const = 0;
	};
}