#pragma once

#include <vec3.hpp>

namespace leap::audio
{
	struct SoundData3D final
	{
		glm::vec3 position{};
		float minDistance{ 5.0f };
		float maxDistance{ 20.0f };
		float minVolume{ 0.0f };
		float maxVolume{ 1.0f };
	};
}