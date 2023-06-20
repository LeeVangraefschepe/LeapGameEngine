#include "ServiceLocator.h"

#include "Interfaces/AudioSystem.h"

std::unique_ptr<leap::audio::DefaultAudioSystem> leap::ServiceLocator::m_pDefaultAudioSystem{ std::make_unique<leap::audio::DefaultAudioSystem>() };
std::unique_ptr<leap::audio::AudioSystem> leap::ServiceLocator::m_pAudioSystem{};

leap::audio::AudioSystem& leap::ServiceLocator::GetAudio()
{
	return m_pAudioSystem.get() == nullptr ? *m_pDefaultAudioSystem.get() : *m_pAudioSystem.get();
}