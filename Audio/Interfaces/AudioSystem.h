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
		virtual int LoadSound(const std::string& filePath, bool is3DSound) = 0;
		virtual int LoadSoundAsync(const std::string& filePath, bool is3DSound) = 0;
		virtual bool IsValidSound(int id) = 0;
		virtual int PlaySound2D(int id, float volume) = 0;
		virtual int PlaySound3D(int id, const SoundData3D& soundData) = 0;
		virtual void SetVolume2D(int channel, float volume) = 0;
		virtual void UpdateSound3D(int channel, const SoundData3D& soundData) = 0;
		virtual void UpdateListener3D(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& forward, const glm::vec3& up) = 0;
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
		virtual int LoadSound(const std::string&, bool) { return -1; };
		virtual int LoadSoundAsync(const std::string&, bool) { return -1; };
		virtual bool IsValidSound(int) { return false; }
		virtual int PlaySound2D(int, float) { return -1; }
		virtual int PlaySound3D(int, const SoundData3D&) { return -1; }
		virtual void SetVolume2D(int, float){};
		virtual void UpdateSound3D(int, const SoundData3D&){};
		virtual void UpdateListener3D(const glm::vec3&, const glm::vec3&, const glm::vec3&, const glm::vec3&){};
		virtual void Pause(int){};
		virtual void Resume(int){};
		virtual void PauseAll(){};
		virtual void ResumeAll(){};
		virtual void Mute(int){};
		virtual void Unmute(int){};
		virtual void MuteAll(){};
		virtual void UnmuteAll(){};
		virtual void Update(){};
	};
}