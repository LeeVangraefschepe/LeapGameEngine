#include "FmodAudioSystem.h"

leap::audio::FmodAudioSystem::FmodAudioSystem()
{
}

int leap::audio::FmodAudioSystem::LoadSound(const std::string& filePath)
{
	return 0;
}

void leap::audio::FmodAudioSystem::LoadSoundAsync(const std::string& filePath, const std::function<void(int)>& callback)
{
}

void leap::audio::FmodAudioSystem::PlaySound2D(int id, float volume, const std::function<void(int)>& callback)
{
}

void leap::audio::FmodAudioSystem::PlaySound3D(int id, const SoundData3D& soundData, const std::function<void(int)>& callback)
{
}

void leap::audio::FmodAudioSystem::SetVolume2D(int id, float volume)
{
}

void leap::audio::FmodAudioSystem::UpdateSound3D(int id, const SoundData3D& soundData)
{
}

void leap::audio::FmodAudioSystem::UpdateListener3D(const glm::vec3& position)
{
}

void leap::audio::FmodAudioSystem::Pause(int id)
{
}

void leap::audio::FmodAudioSystem::Resume(int id)
{
}

void leap::audio::FmodAudioSystem::PauseAll()
{
}

void leap::audio::FmodAudioSystem::ResumeAll()
{
}

void leap::audio::FmodAudioSystem::Mute(int id)
{
}

void leap::audio::FmodAudioSystem::Unmute(int id)
{
}

void leap::audio::FmodAudioSystem::MuteAll()
{
}

void leap::audio::FmodAudioSystem::UnmuteAll()
{
}
