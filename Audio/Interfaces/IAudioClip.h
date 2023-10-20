#pragma once

namespace leap::audio
{
	class IAudioClip
	{
	public:
		virtual ~IAudioClip() = default;

		virtual int Get3DSound() = 0;
		virtual int Get2DSound() = 0;
	};
}