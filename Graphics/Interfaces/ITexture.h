#pragma once

#include "vec2.hpp"
#include "vec4.hpp"

namespace leap::graphics
{
	class IRenderer;

	class ITexture
	{
	public:
		virtual ~ITexture() = default;

		virtual void SetData(void* pData, unsigned int nrBytes) = 0;
		virtual glm::ivec2 GetSize() const = 0;
	};
}