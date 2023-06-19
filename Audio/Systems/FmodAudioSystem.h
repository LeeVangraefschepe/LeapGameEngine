#pragma once

#include "../Interfaces/AudioSystem.h"

#include <memory>

namespace leap::audio
{
	class FmodAudioSystemPimpl;

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
		virtual int LoadSoundAsync(const std::string& filePath) override;
		virtual bool IsValidSound(int id);
		virtual int PlaySound2D(int id, float volume) override;
		virtual int PlaySound3D(int id, const SoundData3D& soundData) override;
		virtual void SetVolume2D(int channel, float volume) override;
		virtual void UpdateSound3D(int channel, const SoundData3D& soundData) override;
		virtual void UpdateListener3D(const glm::vec3& position) override;
		virtual void Pause(int channel) override;
		virtual void Resume(int channel) override;
		virtual void PauseAll() override;
		virtual void ResumeAll() override;
		virtual void Mute(int channel) override;
		virtual void Unmute(int channel) override;
		virtual void MuteAll() override;
		virtual void UnmuteAll() override;
		virtual void Update() override;

	private:
		std::unique_ptr<FmodAudioSystemPimpl> m_pImpl{};

		const int m_MaxChannels{ 512 };
	};
}