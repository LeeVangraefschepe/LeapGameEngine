#pragma once

#include <functional>

namespace leap::audio
{
	struct ChannelData final
	{
		~ChannelData()
		{
			stopCallback();
		}

		int channelId{};
		bool paused{};
		bool muted{};
		std::function<void()> stopCallback{};
	};
}