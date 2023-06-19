#pragma once

#include "../Interfaces/AudioSystem.h"

namespace FMOD
{
	class System;
}

namespace leap::audio
{
	class FmodAudioSystem final : public AudioSystem
	{
	public:
		FmodAudioSystem();
		virtual ~FmodAudioSystem();

		FmodAudioSystem(const FmodAudioSystem& other) = delete;
		FmodAudioSystem(FmodAudioSystem&& other) = delete;
		FmodAudioSystem& operator=(const FmodAudioSystem& other) = delete;
		FmodAudioSystem& operator=(FmodAudioSystem&& other) = delete;

		virtual int LoadSound(const std::string& filePath) override;
		virtual void LoadSoundAsync(const std::string& filePath, const std::function<void(int)>& callback) override;
		virtual void PlaySound2D(int id, float volume, const std::function<void(int)>& callback) override;
		virtual void PlaySound3D(int id, const SoundData3D& soundData, const std::function<void(int)>& callback) override;
		virtual void SetVolume2D(int id, float volume) override;
		virtual void UpdateSound3D(int id, const SoundData3D& soundData) override;
		virtual void UpdateListener3D(const glm::vec3& position) override;
		virtual void Pause(int id) override;
		virtual void Resume(int id) override;
		virtual void PauseAll() override;
		virtual void ResumeAll() override;
		virtual void Mute(int id) override;
		virtual void Unmute(int id) override;
		virtual void MuteAll() override;
		virtual void UnmuteAll() override;
		virtual void Update() override;

	private:
		FMOD::System* m_pSystem{};

		const int m_MaxChannels{ 512 };
	};
}