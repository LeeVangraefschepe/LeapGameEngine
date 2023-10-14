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

		void SetClip(audio::IAudioClip* clipPath);
		void Set3DSound(bool is3DSound);
		void SetVolume(float volume);
		void SetPlayOnAwake(bool playOnAwake);

	protected:
		virtual void Awake() override;
		virtual void Update() override;

	private:
		void Update2DVolume() const;
		void Update3DSound() const;

		audio::IAudioClip* m_pClip{};
		bool m_Is3DSound{};
		float m_Volume{ 1.0f };
		bool m_PlayOnAwake{};

		int m_Channel{ -1 };
	};
}