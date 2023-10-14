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
	if (!m_pClip) return;

	m_Channel = ServiceLocator::GetAudio().PlaySound(m_pClip, m_Is3DSound, [this]() { m_Channel = -1; });
	if (m_Is3DSound)
	{
		Update3DSound();
	}
	else
	{
		Update2DVolume();
	}
}

void leap::AudioSource::Stop()
{
	ServiceLocator::GetAudio().Stop(m_Channel);
}

void leap::AudioSource::Set3DSound(bool is3DSound)
{
	m_Is3DSound = is3DSound;
}

void leap::AudioSource::SetVolume(float volume)
{
	m_Volume = volume;

	if (m_Channel < 0) return;

	Update2DVolume();
}

void leap::AudioSource::SetPlayOnAwake(bool playOnAwake)
{
	m_PlayOnAwake = playOnAwake;
}

void leap::AudioSource::Awake()
{
	if (m_PlayOnAwake) Play();
}

void leap::AudioSource::Update()
{
	if (m_Channel < 0) return;
	if (!m_Is3DSound) return;

	Update3DSound();
}

void leap::AudioSource::Update2DVolume() const
{
	ServiceLocator::GetAudio().SetVolume2D(m_Channel, m_Volume);
}

void leap::AudioSource::Update3DSound() const
{
	audio::SoundData3D data
	{
		GetTransform()->GetWorldPosition()
	};

	ServiceLocator::GetAudio().UpdateSound3D(m_Channel, data);
}
