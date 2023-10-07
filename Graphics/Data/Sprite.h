#pragma once

#include "SpriteVertex.h"
#include "../Interfaces/ITexture.h"

namespace leap::graphics
{
	struct Sprite final
	{
		SpriteVertex vertex{};
		ITexture* pTexture{};
	};
}