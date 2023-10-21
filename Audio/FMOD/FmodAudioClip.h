#pragma once

#include "../Interfaces/IAudioClip.h"

#include <string>

namespace leap::audio
{
	class FmodAudioSystem;

	class FmodAudioClip final : public IAudioClip
	{
	public:
		FmodAudioClip(FmodAudioSystem* pSystem, const std::string& path, bool preLoad);
		virtual ~FmodAudioClip() = default;

		virtual int Get3DSound() override;
		virtual int Get2DSound() override;

	private:
		void LoadSound(bool is3dSound);

		std::string m_Path{};

		int m_Index3dSound{ -1 };
		int m_Index2dSound{ -1 };

		FmodAudioSystem* m_pSystem{};
	};
}