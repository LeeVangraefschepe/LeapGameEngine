#include "ServiceLocator.h"

#include "Interfaces/IAudioSystem.h"
#include "Interfaces/IRenderer.h"
#include "Interfaces/IPhysics.h"

std::unique_ptr<leap::audio::DefaultAudioSystem> leap::ServiceLocator::m_pDefaultAudioSystem{ std::make_unique<leap::audio::DefaultAudioSystem>() };
std::unique_ptr<leap::audio::IAudioSystem> leap::ServiceLocator::m_pAudioSystem{};
std::unique_ptr<leap::graphics::DefaultRenderer> leap::ServiceLocator::m_pDefaultRenderer{ std::make_unique<leap::graphics::DefaultRenderer>() };
std::unique_ptr<leap::graphics::IRenderer> leap::ServiceLocator::m_pRenderer{};
std::unique_ptr<leap::physics::DefaultPhysics> leap::ServiceLocator::m_pDefaultPhysics{ std::make_unique<leap::physics::DefaultPhysics>() };
std::unique_ptr<leap::physics::IPhysics> leap::ServiceLocator::m_pPhysics{};

leap::audio::IAudioSystem& leap::ServiceLocator::GetAudio()
{
	return m_pAudioSystem.get() == nullptr ? *m_pDefaultAudioSystem : *m_pAudioSystem;
}

leap::graphics::IRenderer& leap::ServiceLocator::GetRenderer()
{
	return m_pRenderer.get() == nullptr ? *m_pDefaultRenderer : *m_pRenderer;
}

leap::physics::IPhysics& leap::ServiceLocator::GetPhysics()
{
	return m_pPhysics.get() == nullptr ? *m_pDefaultPhysics : *m_pPhysics;
}
