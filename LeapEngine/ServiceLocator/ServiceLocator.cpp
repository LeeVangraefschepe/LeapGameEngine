#include "ServiceLocator.h"

#include "Interfaces/IAudioSystem.h"
#include "Interfaces/IRenderer.h"

std::unique_ptr<leap::audio::DefaultAudioSystem> leap::ServiceLocator::m_pDefaultAudioSystem{ std::make_unique<leap::audio::DefaultAudioSystem>() };
std::unique_ptr<leap::audio::IAudioSystem> leap::ServiceLocator::m_pAudioSystem{};
std::unique_ptr<leap::graphics::DefaultRenderer> leap::ServiceLocator::m_pDefaultRenderer{ std::make_unique<leap::graphics::DefaultRenderer>() };
std::unique_ptr<leap::graphics::IRenderer> leap::ServiceLocator::m_pRenderer{};

leap::audio::IAudioSystem& leap::ServiceLocator::GetAudio()
{
	return m_pAudioSystem.get() == nullptr ? *m_pDefaultAudioSystem.get() : *m_pAudioSystem.get();
}

leap::graphics::IRenderer& leap::ServiceLocator::GetRenderer()
{
	return m_pRenderer.get() == nullptr ? *m_pDefaultRenderer.get() : *m_pRenderer.get();
}
