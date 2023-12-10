#include "AudioSource.h"

#include "../../ServiceLocator/ServiceLocator.h"

#include "../Transform/Transform.h"

#include <Interfaces/IAudioSystem.h>

void leap::AudioSource::SetClip(audio::IAudioClip* pClip)
{
	m_pClip = pClip;
}

void leap::AudioSource::Play()
{
	if (IsPlaying())
	{
		Stop();
	}

	if (!m_pClip) return;

	ServiceLocator::GetAudio().PlaySound(&m_Channel, m_pClip, m_Is3DSound, 
		[this]() { m_Channel = -1; },
		[this]()
		{
			if (m_Is3DSound)
			{
				Update3DSound();
			}
			else
			{
				Update2DVolume();
			}
			UpdateLoopCount();
			UpdatePauseState();
			UpdateMuteState();
		});
}

void leap::AudioSource::Stop()
{
	if (!IsPlaying()) return;
	ServiceLocator::GetAudio().Stop(m_Channel);
	m_Channel = -1;
}

void leap::AudioSource::Pause()
{
	m_IsPaused = true;

	if(IsPlaying()) UpdatePauseState();
}

void leap::AudioSource::Resume()
{
	m_IsPaused = false;

	if (IsPlaying()) UpdatePauseState();
}

void leap::AudioSource::Mute() 
{
	m_IsMuted = true;

	if (IsPlaying()) UpdateMuteState();
}

void leap::AudioSource::Unmute()
{
	m_IsMuted = false;

	if (IsPlaying()) UpdateMuteState();
}

void leap::AudioSource::Set3DSound(bool is3DSound)
{
	m_Is3DSound = is3DSound;
}

void leap::AudioSource::SetVolume(float volume)
{
	m_MaxVolume = volume;

	if (IsPlaying()) Update2DVolume();
}

void leap::AudioSource::SetPlayOnAwake(bool playOnAwake)
{
	m_PlayOnAwake = playOnAwake;
}

void leap::AudioSource::Set3DDistances(float min, float max)
{
	m_MinDistance = min;
	m_MinDistance = max;
}

void leap::AudioSource::Set3DVolume(float min, float max)
{
	m_MinVolume = min;
	m_MaxVolume = max;
}

void leap::AudioSource::Set3DData(float minDistance, float minVolume, float maxDistance, float maxVolume)
{
	Set3DDistances(minDistance, maxDistance);
	Set3DVolume(minVolume, maxVolume);
}

void leap::AudioSource::SetLooping(bool shouldLoop)
{
	m_ShouldLoop = shouldLoop;

	if (IsPlaying()) UpdateLoopCount();
}

bool leap::AudioSource::IsPlaying() const
{
	return m_Channel >= 0;
}

void leap::AudioSource::Awake()
{
	if (m_PlayOnAwake) Play();

	GetTransform()->OnPositionChanged.AddListener(this, &AudioSource::OnPositionChanged);
}

void leap::AudioSource::OnDestroy()
{
	GetTransform()->OnPositionChanged.RemoveListener(this, &AudioSource::OnPositionChanged);

	if (!IsPlaying()) return;

	Stop();
	m_Channel = -1;
}

void leap::AudioSource::OnPositionChanged() const
{
	// Only update if there is a 3D sound playing
	if (!IsPlaying() || !m_Is3DSound) return;

	Update3DSound();
}

void leap::AudioSource::Update2DVolume() const
{
	ServiceLocator::GetAudio().SetVolume2D(m_Channel, m_MaxVolume);
}

void leap::AudioSource::Update3DSound() const
{
	audio::SoundData3D data
	{
		GetTransform()->GetWorldPosition(),
		m_MinDistance,
		m_MaxDistance,
		m_MinVolume,
		m_MaxVolume
	};

	ServiceLocator::GetAudio().UpdateSound3D(m_Channel, data);
}

void leap::AudioSource::UpdateLoopCount() const
{
	ServiceLocator::GetAudio().SetLooping(m_Channel, m_ShouldLoop);
}

void leap::AudioSource::UpdatePauseState() const
{
	if (m_IsPaused)
	{
		ServiceLocator::GetAudio().Pause(m_Channel);
	}
	else
	{
		ServiceLocator::GetAudio().Resume(m_Channel);
	}
}

void leap::AudioSource::UpdateMuteState() const
{
	if (m_IsMuted)
	{
		ServiceLocator::GetAudio().Mute(m_Channel);
	}
	else
	{
		ServiceLocator::GetAudio().Unmute(m_Channel);
	}
}
