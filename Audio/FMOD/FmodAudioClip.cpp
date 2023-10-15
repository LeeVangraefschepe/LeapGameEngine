#include "FmodAudioClip.h"

#include "FmodAudioSystem.h"

leap::audio::FmodAudioClip::FmodAudioClip(FmodAudioSystem* pSystem, const std::string& path, bool preLoad)
	: m_Path{ path }
	, m_pSystem{ pSystem }
{
	if (preLoad)
	{
		// Load both the 3D and 2D sound
		LoadSound(true);
		LoadSound(false);
	}
}

int leap::audio::FmodAudioClip::Get3DSound()
{
	if (m_Index3dSound < 0) LoadSound(true);

	return m_Index3dSound;
}

int leap::audio::FmodAudioClip::Get2DSound()
{
	if (m_Index2dSound < 0) LoadSound(false);

	return m_Index2dSound;
}

void leap::audio::FmodAudioClip::LoadSound(bool is3dSound)
{
	const int sound{ m_pSystem->LoadSoundAsync(m_Path, is3dSound) };

	if (is3dSound)
	{
		m_Index3dSound = sound;
	}
	else
	{
		m_Index2dSound = sound;
	}
}
