#pragma once

#include "../HelperObjects/SoundData3D.h"

#include <string>
#include <functional>

#include <vec3.hpp>

namespace leap::audio
{
	class AudioSystem
	{
	public:
		virtual int LoadSound(const std::string& filePath) = 0;
		virtual void LoadSoundAsync(const std::string& filePath, const std::function<void(int)>& callback) = 0;
		virtual void PlaySound2D(int id, float volume, const std::function<void(int)>& callback) = 0;
		virtual void PlaySound3D(int id, const SoundData3D& soundData, const std::function<void(int)>& callback) = 0;
		virtual void SetVolume2D(int id, float volume) = 0;
		virtual void UpdateSound3D(int id, const SoundData3D& soundData) = 0;
		virtual void UpdateListener3D(const glm::vec3& position) = 0;
		virtual void Pause(int id) = 0;
		virtual void Resume(int id) = 0;
		virtual void PauseAll() = 0;
		virtual void ResumeAll() = 0;
		virtual void Mute(int id) = 0;
		virtual void Unmute(int id) = 0;
		virtual void MuteAll() = 0;
		virtual void UnmuteAll() = 0;
	};
}