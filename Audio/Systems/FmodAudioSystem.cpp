#include "FmodAudioSystem.h"

#include "../HelperObjects/ChannelData.h"

#include <fmod.hpp>
#include <fmod_errors.h>

#include <iostream>
#include <vector>

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
                std::cout << "FMOD error! " << result << " " << FMOD_ErrorString(result) << "\n";
                return;
            }

            // Initialize FMOD
            result = m_pSystem->init(maxChannels, FMOD_INIT_NORMAL, nullptr);
            if (result != FMOD_OK)
            {
                std::cout << "FMOD error! " << result << " " << FMOD_ErrorString(result) << "\n";
                return;
            }

            std::cout << "FMOD is initialized\n";
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

            // Remove all channels that are no longer playing
            for (auto& sound : m_Sounds)
            {
                auto& channels{ sound.channels };
                for (auto it{ begin(channels) }; it != end(channels);)
                {
                    // Retrieve the current channel
                    FMOD::Channel* pChannel{};
                    FMOD_RESULT result{ m_pSystem->getChannel(it->channelId, &pChannel) };

                    // Throw an error if the channel was not found
                    if (result != FMOD_OK)
                        throw std::runtime_error("FMODAudioSystem Error: Can't retieve channel with this id");

                    // Retrieve the playing state of this channel
                    bool isPlaying{};
                    pChannel->isPlaying(&isPlaying);

                    // If the channel is not playing anymore, erase this channel
                    //  Else continue to the next channel
                    if (!isPlaying)
                    {
                        it = channels.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
        }

        int LoadSound(const std::string& filePath)
        {
            // Create a new sound object and initialize its name and id
            FMODSound sound{ filePath, static_cast<int>(m_Sounds.size()) };

            // Create the FMOD sound in sync
            const FMOD_RESULT result{ m_pSystem->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &sound.pSound) };

            // Throw an error if the sound was not loaded correctly
            if (result != FMOD_OK || !sound.pSound)
                throw std::runtime_error("FMODAudioSystem Error: No sound could be loaded with this filepath");

            // Store the sound object
            m_Sounds.emplace_back(sound);

            // Make sure the FMOD sound is not destroyed by the destructor
            sound.pSound = nullptr;

            // Return the sound id
            return sound.id;
        }

        int LoadSoundAsync(const std::string& filePath)
        {
            // Create a new sound object and initialize its name and id
            FMODSound sound{ filePath, static_cast<int>(m_Sounds.size()) };

            // Create the FMOD sound in sync
            m_pSystem->createSound(filePath.c_str(), FMOD_NONBLOCKING, nullptr, &sound.pSound);

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
                throw std::runtime_error("FMODAudioSystem Error: No sound found with this id");

            // Retrieve the sound state for this id
            FMOD_OPENSTATE soundState{};
            const FMOD_RESULT result{ m_Sounds[id].pSound->getOpenState(&soundState, nullptr, nullptr, nullptr) };

            // Throw an error if the sound was not found
            if (result != FMOD_OK)
                throw std::runtime_error("FMODAudioSystem Error: No sound could be found with this id");

            return soundState == FMOD_OPENSTATE_READY;
        }

        int PlaySound2D(int id, float volume)
        {
            // Throw an error if no sound exists for this id
            if (m_Sounds.size() <= id)
                throw std::runtime_error("FMODAudioSystem Error: No sound found with this id");

            // Retrieve the sound with this id
            FMODSound& sound{ m_Sounds[id] };

            // Play the sound
            FMOD::Channel* pChannel{};
            const FMOD_RESULT result{ m_pSystem->playSound(sound.pSound, nullptr, false, &pChannel) };

            // Throw an error if the sound was not found
            if (result != FMOD_OK)
                throw std::runtime_error("FMODAudioSystem Error: Failed to play a sound");

            // Apply the given volume
            pChannel->setVolume(volume);

            // Retrieve the channel id
            int channel{};
            pChannel->getIndex(&channel);

            // Insert the new channel to the sound
            sound.channels.emplace_back(ChannelData{ channel, false, volume });

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
                throw std::runtime_error("FMODAudioSystem Error: Can't retieve channel with this id");

            // Apply the given volume
            result = pChannel->setVolume(volume);

            // Throw an error if setting the volume failed
            if (result != FMOD_OK)
                throw std::runtime_error("FMODAudioSystem Error: Can't set the volume on this channel");

            // Find the sound that is using this channel
            auto soundIt{ std::find_if(begin(m_Sounds), end(m_Sounds), [channel](const auto& sound)
                {
                    return std::find_if(begin(sound.channels), end(sound.channels),[channel](const auto& channelData)
                        {
                            return channelData.channelId == channel;
                        }) != end(sound.channels);
                }) };

            // Update the volume in the sound
            std::find_if(begin(soundIt->channels), end(soundIt->channels), [channel](const auto& channelData)
                {
                    return channelData.channelId == channel;
                })->volume = volume;
        }

        void Pause(int channel)
        {
            // Retrieve the current channel
            FMOD::Channel* pChannel{};
            FMOD_RESULT result{ m_pSystem->getChannel(channel, &pChannel) };

            // Throw an error if the channel was not found
            if (result != FMOD_OK)
                throw std::runtime_error("FMODAudioSystem Error: Can't retieve channel with this id");

            // Pause this channel
            result = pChannel->setPaused(true);

            // Throw an error if changing the pause state failed
            if (result != FMOD_OK)
                throw std::runtime_error("FMODAudioSystem Error: Can't set the volume on this channel");

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
            // For each channel in each sound
            for (auto& sound : m_Sounds)
            {
                for (const auto& channelData : sound.channels)
                {
                    // Retrieve the current channel
                    FMOD::Channel* pChannel{};
                    FMOD_RESULT result{ m_pSystem->getChannel(channelData.channelId, &pChannel) };

                    // Throw an error if the channel was not found
                    if (result != FMOD_OK)
                        throw std::runtime_error("FMODAudioSystem Error: Can't retieve channel with this id");

                    // Pause this channel
                    result = pChannel->setPaused(true);

                    // Throw an error if changing the pause state failed
                    if (result != FMOD_OK)
                        throw std::runtime_error("FMODAudioSystem Error: Can't set the volume on this channel");
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
                throw std::runtime_error("FMODAudioSystem Error: Can't retieve channel with this id");

            // Pause this channel
            result = pChannel->setPaused(false);

            // Throw an error if changing the pause state failed
            if (result != FMOD_OK)
                throw std::runtime_error("FMODAudioSystem Error: Can't set the volume on this channel");

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
            // For each channel in each sound
            for (auto& sound : m_Sounds)
            {
                for (const auto& channelData : sound.channels)
                {
                    if (channelData.paused) continue;

                    // Retrieve the current channel
                    FMOD::Channel* pChannel{};
                    FMOD_RESULT result{ m_pSystem->getChannel(channelData.channelId, &pChannel) };

                    // Throw an error if the channel was not found
                    if (result != FMOD_OK)
                        throw std::runtime_error("FMODAudioSystem Error: Can't retieve channel with this id");

                    // Pause this channel
                    result = pChannel->setPaused(false);

                    // Throw an error if changing the pause state failed
                    if (result != FMOD_OK)
                        throw std::runtime_error("FMODAudioSystem Error: Can't set the volume on this channel");
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
    m_pImpl = std::make_unique<FmodAudioSystemPimpl>(m_MaxChannels);
}

leap::audio::FmodAudioSystem::~FmodAudioSystem()
{
}

int leap::audio::FmodAudioSystem::LoadSound(const std::string& filePath)
{
    // Delegate the load sound to the pImpl
    return m_pImpl->LoadSound(filePath);
}

int leap::audio::FmodAudioSystem::LoadSoundAsync(const std::string& filePath)
{
    // Delegate the load sound to the pImpl
    return m_pImpl->LoadSoundAsync(filePath);
}

bool leap::audio::FmodAudioSystem::IsValidSound(int id)
{
    return m_pImpl->IsValidSound(id);
}

int leap::audio::FmodAudioSystem::PlaySound2D(int id, float volume)
{
    // Delegate the play sound to the pImpl
    return m_pImpl->PlaySound2D(id, volume);
}

int leap::audio::FmodAudioSystem::PlaySound3D(int id, const SoundData3D& soundData)
{
    return -1;
}

void leap::audio::FmodAudioSystem::SetVolume2D(int channel, float volume)
{
    // Delegate the SetVolume to the pImpl
    m_pImpl->SetVolume2D(channel, volume);
}

void leap::audio::FmodAudioSystem::UpdateSound3D(int channel, const SoundData3D& soundData)
{
}

void leap::audio::FmodAudioSystem::UpdateListener3D(const glm::vec3& position)
{
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
}

void leap::audio::FmodAudioSystem::Unmute(int channel)
{
}

void leap::audio::FmodAudioSystem::MuteAll()
{
}

void leap::audio::FmodAudioSystem::UnmuteAll()
{
}

void leap::audio::FmodAudioSystem::Update()
{
    // Delegate the update to the pImpl
    m_pImpl->Update();
}
