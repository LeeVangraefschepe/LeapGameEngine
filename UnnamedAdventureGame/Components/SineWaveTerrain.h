#pragma once

#include <Components/Component.h>

namespace unag
{
	class SineWaveTerrain final : public leap::Component
	{
	private:
		virtual void Awake() override;
	};
}