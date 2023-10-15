#pragma once

#include "../Interfaces/IAudioSystem.h"

#include <memory>

namespace leap::audio
{
	class FmodAudioSystemPimpl;
	class FmodAudioClip;

	class FmodAudioSystem final : public IAudioSystem
	{
	public:
		FmodAudioSystem();
		virtual ~FmodAudioSystem();

		FmodAudioSystem(const FmodAudioSystem& other) = delete;
		FmodAudioSystem(FmodAudioSystem&& other) = delete;
		FmodAudioSystem& operator=(const FmodAudioSystem& other) = delete;
		FmodAudioSystem& operator=(FmodAudioSystem&& other) = delete;

		virtual IAudioClip* LoadClip(const std::string& filePath, bool preLoad = true) override;
		virtual int LoadSound(const std::string& filePath, bool is3dSound = false) override;
		virtual int LoadSoundAsync(const std::string& filePath, bool is3dSound = false) override;
		virtual bool IsValidSound(int id);
		virtual void PlaySound(int* pChannelIdx, IAudioClip* pClip, bool is3dSound, const std::function<void()>& stopCallback, const std::function<void()>& startCallback) override;
		virtual bool IsPlaying(int channel) override;
		virtual void SetVolume2D(int channel, float volume) override;
		virtual void UpdateSound3D(int channel, const SoundData3D& soundData) override;
		virtual void UpdateListener3D(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& forward, const glm::vec3& up) override;
		virtual void SetLooping(int channel, bool shouldLoop) override;
		virtual void Pause(int channel) override;
		virtual void Resume(int channel) override;
		virtual void PauseAll() override;
		virtual void ResumeAll() override;
		virtual void Mute(int channel) override;
		virtual void Unmute(int channel) override;
		virtual void MuteAll() override;
		virtual void UnmuteAll() override;
		virtual void Stop(int channel) override;
		virtual void Update() override;

	private:
		std::unique_ptr<FmodAudioSystemPimpl> m_pImpl{};

		std::unordered_map<std::string, std::unique_ptr<FmodAudioClip>> m_Clips{};

		const int m_MaxChannels{ 512 };
	};
}