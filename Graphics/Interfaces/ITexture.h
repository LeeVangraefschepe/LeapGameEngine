#pragma once

#include "vec4.hpp"

namespace leap::graphics
{
	class ITexture
	{
	public:
		virtual ~ITexture() = default;

		virtual glm::vec4 GetPixel(int x, int y) = 0;
	};
}