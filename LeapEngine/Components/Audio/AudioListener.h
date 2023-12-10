#pragma once

#include "../Component.h"

namespace leap
{
	class AudioListener final : public Component
	{
	public:
		AudioListener() = default;
		virtual ~AudioListener() = default;

		AudioListener(const AudioListener& other) = delete;
		AudioListener(AudioListener&& other) = delete;
		AudioListener& operator=(const AudioListener& other) = delete;
		AudioListener& operator=(AudioListener&& other) = delete;

	private:
		virtual void Awake() override;
		virtual void OnDestroy() override;
		void OnPositionChanged() const;
	};
}