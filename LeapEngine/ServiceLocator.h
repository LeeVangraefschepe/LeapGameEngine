#pragma once
#include <memory>

namespace leap
{
	namespace audio
	{
		class AudioSystem;
		class DefaultAudioSystem;
	}

	class ServiceLocator final
	{
	public:
		static audio::AudioSystem& GetAudio();
		template <typename T>
		static void RegisterAudioSystem();
	private:
		static std::unique_ptr<audio::AudioSystem> m_pAudioSystem;
		static std::unique_ptr<audio::DefaultAudioSystem> m_pDefaultAudioSystem;
	};

	template<typename T>
	inline void ServiceLocator::RegisterAudioSystem()
	{
		static_assert(std::is_base_of<leap::audio::AudioSystem, T>::value);
		m_pAudioSystem = std::make_unique<T>();
	}
}