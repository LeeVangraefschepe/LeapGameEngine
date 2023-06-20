#pragma once

namespace leap::audio
{
	struct ChannelData final
	{
		int channelId{};
		bool paused{};
		float volume{};
	};
}