#pragma once

#include <functional>

namespace leap::audio
{
	struct ChannelData final
	{
		ChannelData() = default;
		ChannelData(int _channelId, bool _paused, bool _muted, const std::function<void()> _stopCallback)
			: channelId{ _channelId }
			, paused{ _paused }
			, muted{ _muted }
			, stopCallback{ _stopCallback }
		{
		}

		~ChannelData()
		{
			if(stopCallback) stopCallback();
		} 

		/// <summary>
		/// Transformed the copy constructor to work as a move constructor.
		/// Other variable does not have a const specifier so it can be changed
		/// The only reason we did this is because the construct_at function inside the std::vector constructor
		///		wouldn't call the move constructor, but kept calling the deleted copy constructor
		/// </summary>
		ChannelData(ChannelData& other)	
			: channelId{ other.channelId }
			, paused{ other.paused }
			, muted{ other.muted }
			, stopCallback{ other.stopCallback }
		{
			other.stopCallback = nullptr;
		}
		ChannelData& operator=(const ChannelData& other) = delete;
		ChannelData(ChannelData&& other) noexcept
			: channelId{ other.channelId }
			, paused{ other.paused }
			, muted{ other.muted }
			, stopCallback{ other.stopCallback }
		{
			other.stopCallback = nullptr;
		}
		ChannelData& operator=(ChannelData&& other) noexcept
		{
			channelId = other.channelId;
			paused = other.paused;
			muted = other.muted;
			stopCallback = other.stopCallback;

			other.stopCallback = nullptr;

			return *this;
		}

		int channelId{};
		bool paused{};
		bool muted{};
		std::function<void()> stopCallback{};
	};
}