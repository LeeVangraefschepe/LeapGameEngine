#pragma once

#include "../Interfaces/AudioSystem.h"

namespace FMOD
{
	class System;
	class Sound;
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
		struct FMODSound
		{
			FMODSound(const std::string& _name, int _id) : name{ _name }, id{ _id } {}

			FMOD::Sound* pSound{};
			std::string name{};
			int id{};
		};

		FMOD::System* m_pSystem{};
		std::vector<FMODSound> m_Sounds{};

		const int m_MaxChannels{ 512 };
	};
}