#pragma once

#include <Components/Component.h>

namespace unag
{
	class SineWaveTerrain final : public leap::Component
	{
	private:
		//virtual void Awake() override;
		virtual void FixedUpdate() override;

		unsigned int _curIndex = 0;
	};
}