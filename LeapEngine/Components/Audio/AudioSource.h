#pragma once

#include "../Component.h"

#include <string>

namespace leap
{
	namespace audio
	{
		class IAudioClip;
	}

	class AudioSource final : public Component
	{
	public:
		AudioSource() = default;
		virtual ~AudioSource() = default;

		AudioSource(const AudioSource& other) = delete;
		AudioSource(AudioSource&& other) = delete;
		AudioSource& operator=(const AudioSource& other) = delete;
		AudioSource& operator=(AudioSource&& other) = delete;

		void Play();
		void Stop();
		void Pause();
		void Resume();
		void Mute();
		void Unmute();

		void SetClip(audio::IAudioClip* clipPath);
		void Set3DSound(bool is3DSound);
		void SetVolume(float volume);
		void SetPlayOnAwake(bool playOnAwake);
		void Set3DDistances(float min, float max);
		void Set3DVolume(float min, float max);
		void Set3DData(float minDistance, float minVolume, float maxDistance, float maxVolume);
		void SetLooping(bool shouldLoop);

		bool IsPlaying() const;

	private:
		virtual void Awake() override;
		virtual void OnDestroy() override;

		void OnPositionChanged() const;
		void Update2DVolume() const;
		void Update3DSound() const;
		void UpdateLoopCount() const;
		void UpdatePauseState() const;
		void UpdateMuteState() const;

		// General audio data
		audio::IAudioClip* m_pClip{};
		bool m_PlayOnAwake{};
		bool m_Is3DSound{};
		bool m_ShouldLoop{};

		// 3D data
		float m_MinDistance{ 5.0f };
		float m_MaxDistance{ 20.0f };
		float m_MinVolume{ 0.0f };
		// MaxVolume is also used as the volume for 2D sound
		float m_MaxVolume{ 1.0f }; 

		// Runtime data
		int m_Channel{ -1 };
		bool m_IsPaused{};
		bool m_IsMuted{};
	};
}