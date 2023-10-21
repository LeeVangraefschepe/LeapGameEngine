#include "FmodAudioSystem.h"

#include "../HelperObjects/ChannelData.h"
#include "Debug.h"
#include "FmodAudioClip.h"

#include <fmod.hpp>
#include <fmod_errors.h>
#include <vector>
#include <sstream>

namespace leap::audio
{
    class FmodAudioSystemPimpl final
    {
    public:
        FmodAudioSystemPimpl(int maxChannels)
        {
            // Create the main FMOD system object
            FMOD_RESULT result{ FMOD::System_Create(&m_pSystem) };
            if (result != FMOD_OK)
            {
                std::stringstream errorMsg{};
                errorMsg << "FMODAudioSystem Error: " << FMOD_ErrorString(result);

                Debug::LogError(errorMsg.str());
            }

            // Initialize FMOD
            result = m_pSystem->init(maxChannels, FMOD_INIT_NORMAL, nullptr);
            if (result != FMOD_OK)
            {
                std::stringstream errorMsg{};
                errorMsg << "FMODAudioSystem Error: " << FMOD_ErrorString(result);
                Debug::LogError(errorMsg.str());
            }

            Debug::Log("FMODAudioSystem Log: FMOD is initialized");
        }

        ~FmodAudioSystemPimpl()
        {
            // Release FMOD
            if (m_pSystem) m_pSystem->release();
        }

        void Update()
        {
            // Try to play queued sounds and remove them from the queue if they succeed
            const auto removeIt{ std::remove_if(begin(m_SoundsToPlay), end(m_SoundsToPlay), [&](const QueuedSound& soundToPlay) { return TryPlaySound(soundToPlay); }) };
            if(removeIt != end(m_SoundsToPlay)) m_SoundsToPlay.erase(removeIt);

            // Update the internal audio system
            m_pSystem->update();

            // Variables to be reused
            FMOD::Channel* pChannel{};
            bool isPlaying{};

            // Remove all channels that are no longer playing
            for (auto& sound : m_Sounds)
            {
                auto& channels{ sound.channels };
                for (auto it{ begin(channels) }; it != end(channels);)
                {
                    // Retrieve the current channel
                    FMOD_RESULT result{ m_pSystem->getChannel(it->channelId, &pChannel) };

                    // Throw an error if the channel was not found
                    if (result != FMOD_OK)
                        Debug::LogError("FMODAudioSystem Error: Can't retieve channel with this id");

                    // Retrieve the playing state of this channel
                    pChannel->isPlaying(&isPlaying);

                    // If the channel is not playing anymore, erase this channel
                    if (!isPlaying)
                    {
                        it = channels.erase(it);
                        continue;
                    }

                    // This channel is still playing, continue to the next channel
                    ++it;
                }
            }
        }

        int LoadSound(const std::string& filePath, bool is3DSound)
        {
            // Create a new sound object and initialize its name and id
            FMODSound sound{ filePath, static_cast<int>(m_Sounds.size()) };

            // Choose the correct sound mode
            const FMOD_MODE soundMode{ static_cast<FMOD_MODE>((is3DSound ? FMOD_3D : FMOD_DEFAULT) | FMOD_LOOP_NORMAL) };
            
            // Create the FMOD sound in sync
            const FMOD_RESULT result{ m_pSystem->createSound(filePath.c_str(), soundMode, nullptr, &sound.pSound) };

            // Throw an error if the sound was not loaded correctly
            if (result != FMOD_OK || !sound.pSound)
                Debug::LogError("FMODAudioSystem Error: No sound could be loaded with this filepath");

            // Store the sound object
            m_Sounds.emplace_back(sound);

            // Make sure the FMOD sound is not destroyed by the destructor
            sound.pSound = nullptr;

            // Return the sound id
            return sound.id;
        }

        int LoadSoundAsync(const std::string& filePath, bool is3DSound)
        {
            // Create a new sound object and initialize its name and id
            FMODSound sound{ filePath, static_cast<int>(m_Sounds.size()) };

            // Choose the correct sound mode
            const FMOD_MODE soundMode{ static_cast<FMOD_MODE>((is3DSound ? FMOD_3D : FMOD_DEFAULT) | FMOD_LOOP_NORMAL) };

            // Create the FMOD sound in sync
            m_pSystem->createSound(filePath.c_str(), FMOD_NONBLOCKING | soundMode, nullptr, &sound.pSound);

            // Store the sound object
            m_Sounds.emplace_back(sound);

            // Make sure the FMOD sound is not destroyed by the destructor
            sound.pSound = nullptr;

            // Return the sound id
            return sound.id;
        }

        bool IsValidSound(int id) const
        {
            // Throw an error if no sound exists for this id
            if (m_Sounds.size() <= id)
                Debug::LogError("FMODAudioSystem Error: No sound found with this id");

            // Retrieve the sound state for this id
            FMOD_OPENSTATE soundState{};
            const FMOD_RESULT result{ m_Sounds[id].pSound->getOpenState(&soundState, nullptr, nullptr, nullptr) };

            // Throw an error if the sound was not found
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: No sound could be found with this id");

            return soundState == FMOD_OPENSTATE_READY;
        }

        void PlaySound(int* pChannelIdx, IAudioClip* pClip, bool is3dSound, const std::function<void()>& stopCallback, const std::function<void()>& startCallback)
        {
            const int id{ is3dSound ? pClip->Get3DSound() : pClip->Get2DSound() };

            // Throw an error if no sound exists for this id
            if (m_Sounds.size() <= id)
                Debug::LogError("FMODAudioSystem Error: No sound found with this id");

            const QueuedSound queuedSound{ id, is3dSound, stopCallback, startCallback, pChannelIdx };
            const bool isPlaying{ TryPlaySound(queuedSound) };

            if (!isPlaying) m_SoundsToPlay.emplace_back(queuedSound);
        }

        void SetVolume2D(int channel, float volume) const
        {
            // Retrieve the current channel
            FMOD::Channel* pChannel{};
            FMOD_RESULT result{ m_pSystem->getChannel(channel, &pChannel) };

            // Throw an error if the channel was not found
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't retieve channel with this id");

            // Apply the given volume
            result = pChannel->setVolume(volume);

            // Throw an error if setting the volume failed
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't set the volume on this channel");
        }

        void UpdateSound3D(int channel, const SoundData3D& soundData) const
        {
            // Retrieve the current channel
            FMOD::Channel* pChannel{};
            FMOD_RESULT result{ m_pSystem->getChannel(channel, &pChannel) };

            // Throw an error if the channel was not found
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't retieve channel with this id");

            // Set the 3D attributes for this channel
            const FMOD_VECTOR position{ soundData.position.x, soundData.position.y, soundData.position.z };
            const FMOD_VECTOR velocity{};
            result = pChannel->set3DAttributes(&position, &velocity);

            // Throw an error if changing the 3D attributes failed
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't set the volume on this channel");

            // Set the min and max distance for this channel
            result = pChannel->set3DMinMaxDistance(soundData.minDistance, soundData.maxDistance);

            // Throw an error if changing the min/max attributes failed
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't set the volume on this channel");
        }

        void UpdateListener3D(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& forward, const glm::vec3& up) const
        {
            // Set the 3D attributes of the listener
            const FMOD_VECTOR fmodPosition{ position.x, position.y, position.z };
            const FMOD_VECTOR fmodVelocity{ velocity.x, velocity.y, velocity.z };
            const FMOD_VECTOR fmodForward{ forward.x, forward.y, forward.z };
            const FMOD_VECTOR fmodUp{ up.x, up.y, up.z };
            const FMOD_RESULT result{ m_pSystem->set3DListenerAttributes(0, &fmodPosition, &fmodVelocity, &fmodForward, &fmodUp) };

            // Throw an error if changing the min/max attributes failed
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't set the attributes of the audio listener");
        }

        bool IsPlaying(int channel) const
        {
            // Find the sound that is using this channel
            auto soundIt{ std::find_if(begin(m_Sounds), end(m_Sounds), [channel](const auto& sound)
                {
                    return std::find_if(begin(sound.channels), end(sound.channels),[channel](const auto& channelData)
                        {
                            return channelData.channelId == channel;
                        }) != end(sound.channels);
                }) };

            // If a sound is found, this channel is playing
            return soundIt != end(m_Sounds);
        }

        void SetLooping(int channel, bool shouldLoop) const
        {
            // Retrieve the current channel
            FMOD::Channel* pChannel{};
            FMOD_RESULT result{ m_pSystem->getChannel(channel, &pChannel) };

            // Throw an error if the channel was not found
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't retieve channel with this id");

            // Set the loop count for this channel (-1 is infinite looping, 0 is one shot)
            result = pChannel->setLoopCount(shouldLoop ? -1 : 0);

            // Throw an error if changing the loop count failed
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't change the loop count on this channel");
        }

        void Pause(int channel)
        {
            // Retrieve the current channel
            FMOD::Channel* pChannel{};
            FMOD_RESULT result{ m_pSystem->getChannel(channel, &pChannel) };

            // Throw an error if the channel was not found
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't retieve channel with this id");

            // Pause this channel
            result = pChannel->setPaused(true);

            // Throw an error if changing the pause state failed
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't set the volume on this channel");

            // Find the sound that is using this channel
            auto soundIt{ std::find_if(begin(m_Sounds), end(m_Sounds), [channel](const auto& sound)
                {
                    return std::find_if(begin(sound.channels), end(sound.channels),[channel](const auto& channelData)
                        { 
                            return channelData.channelId == channel; 
                        }) != end(sound.channels);
                }) };

            // Update the pause state in the sound
            std::find_if(begin(soundIt->channels), end(soundIt->channels), [channel](const auto& channelData)
                {
                    return channelData.channelId == channel;
                })->paused = true;
        }

        void PauseAll() const
        {
            // Channel to be reused
            FMOD::Channel* pChannel{};

            // For each channel in each sound
            for (auto& sound : m_Sounds)
            {
                for (const auto& channelData : sound.channels)
                {
                    // Retrieve the current channel
                    FMOD_RESULT result{ m_pSystem->getChannel(channelData.channelId, &pChannel) };

                    // Throw an error if the channel was not found
                    if (result != FMOD_OK)
                        Debug::LogError("FMODAudioSystem Error: Can't retieve channel with this id");

                    // Pause this channel
                    result = pChannel->setPaused(true);

                    // Throw an error if changing the pause state failed
                    if (result != FMOD_OK)
                        Debug::LogError("FMODAudioSystem Error: Can't set the volume on this channel");
                }
            }
        }

        void Resume(int channel)
        {
            // Retrieve the current channel
            FMOD::Channel* pChannel{};
            FMOD_RESULT result{ m_pSystem->getChannel(channel, &pChannel) };

            // Throw an error if the channel was not found
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't retieve channel with this id");

            // Pause this channel
            result = pChannel->setPaused(false);

            // Throw an error if changing the pause state failed
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't set the volume on this channel");

            // Find the sound that is using this channel
            auto soundIt{ std::find_if(begin(m_Sounds), end(m_Sounds), [channel](const auto& sound)
                {
                    return std::find_if(begin(sound.channels), end(sound.channels),[channel](const auto& channelData)
                        {
                            return channelData.channelId == channel;
                        }) != end(sound.channels);
                }) };

            // Update the pause state in the sound
            std::find_if(begin(soundIt->channels), end(soundIt->channels), [channel](const auto& channelData)
                {
                    return channelData.channelId == channel;
                })->paused = false;
        }

        void ResumeAll() const
        {
            // channel to be reused
            FMOD::Channel* pChannel{};

            // For each channel in each sound
            for (auto& sound : m_Sounds)
            {
                for (const auto& channelData : sound.channels)
                {
                    if (channelData.paused) continue;

                    // Retrieve the current channel
                    FMOD_RESULT result{ m_pSystem->getChannel(channelData.channelId, &pChannel) };

                    // Throw an error if the channel was not found
                    if (result != FMOD_OK)
                        Debug::LogError("FMODAudioSystem Error: Can't retieve channel with this id");

                    // Pause this channel
                    result = pChannel->setPaused(false);

                    // Throw an error if changing the pause state failed
                    if (result != FMOD_OK)
                        Debug::LogError("FMODAudioSystem Error: Can't set the volume on this channel");
                }
            }
        }

        void Mute(int channel)
        {
            // Retrieve the current channel
            FMOD::Channel* pChannel{};
            FMOD_RESULT result{ m_pSystem->getChannel(channel, &pChannel) };

            // Throw an error if the channel was not found
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't retieve channel with this id");

            // Mute this channel
            result = pChannel->setMute(true);

            // Throw an error if changing the mute state failed
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't mute this channel");

            // Find the sound that is using this channel
            auto soundIt{ std::find_if(begin(m_Sounds), end(m_Sounds), [channel](const auto& sound)
                {
                    return std::find_if(begin(sound.channels), end(sound.channels),[channel](const auto& channelData)
                        {
                            return channelData.channelId == channel;
                        }) != end(sound.channels);
                }) };

            // Update the mute state in the sound
            std::find_if(begin(soundIt->channels), end(soundIt->channels), [channel](const auto& channelData)
                {
                    return channelData.channelId == channel;
                })->muted = true;
        }

        void MuteAll() const
        {
            // Channel to be reused
            FMOD::Channel* pChannel{};

            // For each channel in each sound
            for (auto& sound : m_Sounds)
            {
                for (const auto& channelData : sound.channels)
                {
                    // Retrieve the current channel
                    FMOD_RESULT result{ m_pSystem->getChannel(channelData.channelId, &pChannel) };

                    // Throw an error if the channel was not found
                    if (result != FMOD_OK)
                        Debug::LogError("FMODAudioSystem Error: Can't retieve channel with this id");

                    // Unmute this channel
                    result = pChannel->setMute(true);

                    // Throw an error if changing the mute state failed
                    if (result != FMOD_OK)
                        Debug::LogError("FMODAudioSystem Error: Can't unmute this channel");
                }
            }
        }

        void Unmute(int channel)
        {
            // Retrieve the current channel
            FMOD::Channel* pChannel{};
            FMOD_RESULT result{ m_pSystem->getChannel(channel, &pChannel) };

            // Throw an error if the channel was not found
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't retieve channel with this id");

            // Unmute this channel
            result = pChannel->setMute(false);

            // Throw an error if changing the mute state failed
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't unmute this channel");

            // Find the sound that is using this channel
            auto soundIt{ std::find_if(begin(m_Sounds), end(m_Sounds), [channel](const auto& sound)
                {
                    return std::find_if(begin(sound.channels), end(sound.channels),[channel](const auto& channelData)
                        {
                            return channelData.channelId == channel;
                        }) != end(sound.channels);
                }) };

            // Update the mute state in the sound
            std::find_if(begin(soundIt->channels), end(soundIt->channels), [channel](const auto& channelData)
                {
                    return channelData.channelId == channel;
                })->muted = false;
        }

        void UnmuteAll() const
        {
            // Channel to be reused
            FMOD::Channel* pChannel{};

            // For each channel in each sound
            for (auto& sound : m_Sounds)
            {
                for (const auto& channelData : sound.channels)
                {
                    if (channelData.muted) continue;

                    // Retrieve the current channel
                    FMOD_RESULT result{ m_pSystem->getChannel(channelData.channelId, &pChannel) };

                    // Throw an error if the channel was not found
                    if (result != FMOD_OK)
                        Debug::LogError("FMODAudioSystem Error: Can't retieve channel with this id");

                    // Unmute this channel
                    result = pChannel->setMute(false);

                    // Throw an error if changing the mute state failed
                    if (result != FMOD_OK)
                        Debug::LogError("FMODAudioSystem Error: Can't unmute this channel");
                }
            }
        }

        void Stop(int channel)
        {
            // Retrieve the current channel
            FMOD::Channel* pChannel{};
            FMOD_RESULT result{ m_pSystem->getChannel(channel, &pChannel) };

            // Throw an error if the channel was not found
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't retieve channel with this id");

            // Stop this channel
            result = pChannel->stop();

            // Throw an error if stopping failed
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't unmute this channel");

            // Find the sound that is using this channel
            auto soundIt{ std::find_if(begin(m_Sounds), end(m_Sounds), [channel](const auto& sound)
                {
                    return std::find_if(begin(sound.channels), end(sound.channels),[channel](const auto& channelData)
                        {
                            return channelData.channelId == channel;
                        }) != end(sound.channels);
                }) };

            // Remove the channel from the sound that is playing
            soundIt->channels.erase(std::remove_if(begin(soundIt->channels), end(soundIt->channels), [channel](const auto& channelData)
                {
                    return channelData.channelId == channel;
                }));
        }

    private:
        struct FMODSound final
        {
            FMODSound(const std::string& _name, int _id) : name{ _name }, id{ _id } {}

            FMOD::Sound* pSound{};
            std::string name{};
            int id{};
            std::vector<ChannelData> channels{};
        };

        struct QueuedSound final
        {
            int id{};
            bool is3dSound{};
            std::function<void()> stopCallback{};
            std::function<void()> startCallback{};
            int* pChannelIdx{};
        };

        bool TryPlaySound(const QueuedSound& queuedSound)
        {
            // Retrieve the sound with this id
            FMODSound& sound{ m_Sounds[queuedSound.id] };

            // Play the sound
            FMOD::Channel* pChannel{};
            FMOD_RESULT result{ m_pSystem->playSound(sound.pSound, nullptr, false, &pChannel) };

            // Throw an error if the sound was not found
            if (result != FMOD_OK)
            {
                if (result == FMOD_ERR_NOTREADY) *queuedSound.pChannelIdx = -1;
                else Debug::LogError("FMODAudioSystem Error: Failed to play a sound");

                return false;
            }

            if (queuedSound.is3dSound)
            {
                // Set the roll off mode of this channel
                result = pChannel->setMode(FMOD_3D_LINEARROLLOFF);

                // Throw an error if settings the roll of mode failed
                if (result != FMOD_OK)
                    Debug::LogError("FMODAudioSystem Error: Failed to set roll off mode for this channel");
            }

            // Retrieve the channel id
            int channel{};
            pChannel->getIndex(&channel);

            // Insert the new channel to the sound
            sound.channels.emplace_back(channel, false, false, queuedSound.stopCallback);

            // Set the current playing channel
            *queuedSound.pChannelIdx = channel;

            queuedSound.startCallback();

            return true;
        }

        FMOD::System* m_pSystem{};
        std::vector<FMODSound> m_Sounds{};
        std::vector<QueuedSound> m_SoundsToPlay{};
    };
}

leap::audio::FmodAudioSystem::FmodAudioSystem()
{
    Debug::Log("FMODAudioSystem Log: Created FMOD system");
    m_pImpl = std::make_unique<FmodAudioSystemPimpl>(m_MaxChannels);
}

leap::audio::FmodAudioSystem::~FmodAudioSystem()
{
    Debug::Log("FMODAudioSystem Log: Destroyed FMOD system");
}

leap::audio::IAudioClip* leap::audio::FmodAudioSystem::LoadClip(const std::string& filePath, bool preLoad)
{
    if (m_Clips.contains(filePath)) return m_Clips[filePath].get();

    auto pClip{ std::make_unique<FmodAudioClip>(this, filePath, preLoad) };
    auto pClipRaw{ pClip.get() };

    m_Clips[filePath] = std::move(pClip);

    return pClipRaw;
}

int leap::audio::FmodAudioSystem::LoadSound(const std::string& filePath, bool is3dSound)
{
    return m_pImpl->LoadSound(filePath, is3dSound);
}

int leap::audio::FmodAudioSystem::LoadSoundAsync(const std::string& filePath, bool is3dSound)
{
    return m_pImpl->LoadSoundAsync(filePath, is3dSound);
}

bool leap::audio::FmodAudioSystem::IsValidSound(int id)
{
    return m_pImpl->IsValidSound(id);
}

void leap::audio::FmodAudioSystem::PlaySound(int* pChannelIdx, IAudioClip* pClip, bool is3dSound, const std::function<void()>& stopCallback, const std::function<void()>& startCallback)
{
    m_pImpl->PlaySound(pChannelIdx, pClip, is3dSound, stopCallback, startCallback);
}

bool leap::audio::FmodAudioSystem::IsPlaying(int channel)
{
    return m_pImpl->IsPlaying(channel);
}

void leap::audio::FmodAudioSystem::SetVolume2D(int channel, float volume)
{
    m_pImpl->SetVolume2D(channel, volume);
}

void leap::audio::FmodAudioSystem::UpdateSound3D(int channel, const SoundData3D& soundData)
{
    m_pImpl->UpdateSound3D(channel, soundData);
}

void leap::audio::FmodAudioSystem::UpdateListener3D(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& forward, const glm::vec3& up)
{
    m_pImpl->UpdateListener3D(position, velocity, forward, up);
}

void leap::audio::FmodAudioSystem::Pause(int channel)
{
    m_pImpl->Pause(channel);
}

void leap::audio::FmodAudioSystem::SetLooping(int channel, bool shouldLoop)
{
    m_pImpl->SetLooping(channel, shouldLoop);
}

void leap::audio::FmodAudioSystem::Resume(int channel)
{
    m_pImpl->Resume(channel);
}

void leap::audio::FmodAudioSystem::PauseAll()
{
    m_pImpl->PauseAll();
}

void leap::audio::FmodAudioSystem::ResumeAll()
{
    m_pImpl->ResumeAll();
}

void leap::audio::FmodAudioSystem::Mute(int channel)
{
    m_pImpl->Mute(channel);
}

void leap::audio::FmodAudioSystem::Unmute(int channel)
{
    m_pImpl->Unmute(channel);
}

void leap::audio::FmodAudioSystem::MuteAll()
{
    m_pImpl->MuteAll();
}

void leap::audio::FmodAudioSystem::UnmuteAll()
{
    m_pImpl->UnmuteAll();
}

void leap::audio::FmodAudioSystem::Stop(int channel)
{
    m_pImpl->Stop(channel);
}

void leap::audio::FmodAudioSystem::Update()
{
    m_pImpl->Update();
}
