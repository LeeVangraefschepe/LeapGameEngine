#pragma once

#include "vec2.hpp"
#include "vec4.hpp"
#include <vector>

namespace leap::graphics
{
	class IRenderer;

	class ITexture
	{
	public:
		virtual ~ITexture() = default;

		virtual void SetData(const void* pData, unsigned int nrBytes) = 0;
		virtual std::vector<unsigned char> GetData() = 0;
		virtual glm::uvec2 GetSize() const = 0;
		virtual void Remove() = 0;
	};
}