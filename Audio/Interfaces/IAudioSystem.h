#pragma once

#include "../HelperObjects/SoundData3D.h"

#include <string>
#include <functional>

#include <vec3.hpp>

namespace leap::audio
{
	class IAudioClip;

	class IAudioSystem
	{
	public:
		virtual ~IAudioSystem() = default;
		virtual IAudioClip* LoadClip(const std::string& filePath, bool preLoad = true) = 0;
		virtual int LoadSound(const std::string& filePath, bool is3dSound) = 0;
		virtual int LoadSoundAsync(const std::string& filePath, bool is3dSound) = 0;
		virtual bool IsValidSound(int id) = 0;
		virtual void PlaySound(int* pChannelIdx, IAudioClip* pClip, bool is3dSound, const std::function<void()>& stopCallback, const std::function<void()>& startCallback) = 0;
		virtual bool IsPlaying(int channel) = 0;
		virtual void SetVolume2D(int channel, float volume) = 0;
		virtual void UpdateSound3D(int channel, const SoundData3D& soundData) = 0;
		virtual void UpdateListener3D(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& forward, const glm::vec3& up) = 0;
		virtual void SetLooping(int channel, bool shouldLoop) = 0;
		virtual void Pause(int channel) = 0;
		virtual void Resume(int channel) = 0;
		virtual void PauseAll() = 0;
		virtual void ResumeAll() = 0;
		virtual void Mute(int channel) = 0;
		virtual void Unmute(int channel) = 0;
		virtual void MuteAll() = 0;
		virtual void UnmuteAll() = 0;
		virtual void Stop(int channel) = 0;
		virtual void Update() = 0;
	};

	class DefaultAudioSystem final : public IAudioSystem
	{
	public:
		DefaultAudioSystem() = default;
		virtual ~DefaultAudioSystem() = default;
		virtual IAudioClip* LoadClip(const std::string&, bool) { return nullptr; }
		virtual int LoadSound(const std::string&, bool) { return -1; };
		virtual int LoadSoundAsync(const std::string&, bool) { return -1; };
		virtual bool IsValidSound(int) { return false; }
		virtual void PlaySound(int*, IAudioClip*, bool, const std::function<void()>&, const std::function<void()>&){}
		virtual bool IsPlaying(int) { return false; }
		virtual void SetVolume2D(int, float){};
		virtual void UpdateSound3D(int, const SoundData3D&){};
		virtual void UpdateListener3D(const glm::vec3&, const glm::vec3&, const glm::vec3&, const glm::vec3&){};
		virtual void SetLooping(int, bool){};
		virtual void Pause(int){};
		virtual void Resume(int){};
		virtual void PauseAll(){};
		virtual void ResumeAll(){};
		virtual void Mute(int){};
		virtual void Unmute(int){};
		virtual void MuteAll(){};
		virtual void UnmuteAll(){};
		virtual void Stop(int){};
		virtual void Update(){};
	};
}