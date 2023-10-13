#include "FmodAudioSystem.h"

#include "../HelperObjects/ChannelData.h"
#include "Debug.h"

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
            const FMOD_MODE soundMode{ static_cast<FMOD_MODE>(is3DSound ? FMOD_3D : FMOD_DEFAULT) };
            
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
            const FMOD_MODE soundMode{ static_cast<FMOD_MODE>(is3DSound ? FMOD_3D : FMOD_DEFAULT) };

            // Create the FMOD sound in sync
            m_pSystem->createSound(filePath.c_str(), FMOD_NONBLOCKING | soundMode, nullptr, &sound.pSound);

            // Store the sound object
            m_Sounds.emplace_back(sound);

            // Make sure the FMOD sound is not destroyed by the destructor
            sound.pSound = nullptr;

            // Return the sound id
            return sound.id;
        }

        bool IsValidSound(int id)
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

        int PlaySound2D(int id, float volume)
        {
            // Throw an error if no sound exists for this id
            if (m_Sounds.size() <= id)
                Debug::LogError("FMODAudioSystem Error: No sound found with this id");

            // Retrieve the sound with this id
            FMODSound& sound{ m_Sounds[id] };

            // Play the sound
            FMOD::Channel* pChannel{};
            const FMOD_RESULT result{ m_pSystem->playSound(sound.pSound, nullptr, false, &pChannel) };

            // Throw an error if the sound was not found
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Failed to play a sound");

            // Apply the given volume
            pChannel->setVolume(volume);

            // Retrieve the channel id
            int channel{};
            pChannel->getIndex(&channel);

            // Insert the new channel to the sound
            sound.channels.emplace_back(ChannelData{ channel, false, false });

            // Return the current playing channel
            return channel;
        }

        void SetVolume2D(int channel, float volume)
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

        int PlaySound3D(int id, const SoundData3D& soundData)
        {
            // Throw an error if no sound exists for this id
            if (m_Sounds.size() <= id)
                Debug::LogError("FMODAudioSystem Error: No sound found with this id");

            // Retrieve the sound with this id
            FMODSound& sound{ m_Sounds[id] };

            // Play the sound
            FMOD::Channel* pChannel{};
            FMOD_RESULT result{ m_pSystem->playSound(sound.pSound, nullptr, false, &pChannel) };

            // Throw an error if the sound was not found
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Failed to play a sound");

            // Set the roll off mode of this channel
            result = pChannel->setMode(FMOD_3D_LINEARSQUAREROLLOFF);

            // Throw an error if settings the roll of mode failed
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Failed to set roll off mode for this channel");

            // Retrieve the channel id
            int channel{};
            pChannel->getIndex(&channel);

            // Insert the new channel to the sound
            sound.channels.emplace_back(ChannelData{ channel, false, false });

            // Update the 3D sound
            UpdateSound3D(channel, soundData);

            // Return the current playing channel
            return channel;
        }

        void UpdateSound3D(int channel, const SoundData3D& soundData)
        {
            // Retrieve the current channel
            FMOD::Channel* pChannel{};
            FMOD_RESULT result{ m_pSystem->getChannel(channel, &pChannel) };

            // Throw an error if the channel was not found
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't retieve channel with this id");

            // Set the 3D attributes for this channel
            const FMOD_VECTOR position{ soundData.position.x, soundData.position.y, soundData.position.z };
            result = pChannel->set3DAttributes(&position, nullptr);

            // Throw an error if changing the 3D attributes failed
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't set the volume on this channel");

            // Set the min and max distance for this channel
            result = pChannel->set3DMinMaxDistance(soundData.minDistance, soundData.maxDistance);

            // Throw an error if changing the min/max attributes failed
            if (result != FMOD_OK)
                Debug::LogError("FMODAudioSystem Error: Can't set the volume on this channel");
        }

        void UpdateListener3D(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& forward, const glm::vec3& up)
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

        bool IsPlaying(int channel)
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

        void PauseAll()
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

        void ResumeAll()
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

        void MuteAll()
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

        void UnmuteAll()
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

    private:
        struct FMODSound
        {
            FMODSound(const std::string& _name, int _id) : name{ _name }, id{ _id } {}

            FMOD::Sound* pSound{};
            std::string name{};
            int id{};
            std::vector<ChannelData> channels{};
        };

        FMOD::System* m_pSystem{};
        std::vector<FMODSound> m_Sounds{};
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

int leap::audio::FmodAudioSystem::LoadSound(const std::string& filePath, bool is3DSound)
{
    return m_pImpl->LoadSound(filePath, is3DSound);
}

int leap::audio::FmodAudioSystem::LoadSoundAsync(const std::string& filePath, bool is3DSound)
{
    return m_pImpl->LoadSoundAsync(filePath, is3DSound);
}

bool leap::audio::FmodAudioSystem::IsValidSound(int id)
{
    return m_pImpl->IsValidSound(id);
}

int leap::audio::FmodAudioSystem::PlaySound2D(int id, float volume)
{
    return m_pImpl->PlaySound2D(id, volume);
}

int leap::audio::FmodAudioSystem::PlaySound3D(int id, const SoundData3D& soundData)
{
    return m_pImpl->PlaySound3D(id, soundData);
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

void leap::audio::FmodAudioSystem::Update()
{
    m_pImpl->Update();
}
