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
		virtual ~AudioSystem() = default;
		virtual int LoadSound(const std::string& filePath) = 0;
		virtual int LoadSoundAsync(const std::string& filePath) = 0;
		virtual bool IsValidSound(int id) = 0;
		virtual int PlaySound2D(int id, float volume) = 0;
		virtual int PlaySound3D(int id, const SoundData3D& soundData) = 0;
		virtual void SetVolume2D(int channel, float volume) = 0;
		virtual void UpdateSound3D(int channel, const SoundData3D& soundData) = 0;
		virtual void UpdateListener3D(const glm::vec3& position) = 0;
		virtual void Pause(int channel) = 0;
		virtual void Resume(int channel) = 0;
		virtual void PauseAll() = 0;
		virtual void ResumeAll() = 0;
		virtual void Mute(int channel) = 0;
		virtual void Unmute(int channel) = 0;
		virtual void MuteAll() = 0;
		virtual void UnmuteAll() = 0;
		virtual void Update() = 0;
	};

	class DefaultAudioSystem final : public AudioSystem
	{
	public:
		DefaultAudioSystem() = default;
		virtual ~DefaultAudioSystem() = default;
		virtual int LoadSound(const std::string& filePath) { return -1; };
		virtual int LoadSoundAsync(const std::string& filePath) { return -1; };
		virtual bool IsValidSound(int id) { return false; }
		virtual int PlaySound2D(int id, float volume) { return -1; }
		virtual int PlaySound3D(int id, const SoundData3D& soundData) { return -1; }
		virtual void SetVolume2D(int channel, float volume){};
		virtual void UpdateSound3D(int channel, const SoundData3D& soundData){};
		virtual void UpdateListener3D(const glm::vec3& position){};
		virtual void Pause(int channel){};
		virtual void Resume(int channel){};
		virtual void PauseAll(){};
		virtual void ResumeAll(){};
		virtual void Mute(int channel){};
		virtual void Unmute(int channel){};
		virtual void MuteAll(){};
		virtual void UnmuteAll(){};
		virtual void Update(){};
	};
}