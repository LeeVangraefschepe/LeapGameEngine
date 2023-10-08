#pragma once

#include "SpriteVertex.h"
#include "../Interfaces/ITexture.h"

#include <functional>

namespace leap::graphics
{
	struct Sprite final
	{
		SpriteVertex vertex{};
		ITexture* pTexture{};
		std::function<void()> OnDraw{};
	};
}