#include "FmodAudioSystem.h"

#include <fmod.hpp>
#include <fmod_errors.h>

#include <iostream>

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
        }

        int LoadSound(const std::string& filePath)
        {
            // Create a new sound object and initialize its name and id
            FMODSound sound{ filePath, static_cast<int>(m_Sounds.size()) };

            // Create the FMOD sound in sync
            m_pSystem->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &sound.pSound);

            // Throw an error if the sound was not loaded correctly
            if (!sound.pSound)
                throw std::runtime_error("FMODAudioSystem Error: No sound could be loaded with this filepath");

            // Store the sound object
            m_Sounds.emplace_back(sound);

            // Make sure the FMOD sound is not destroyed by the destructor
            sound.pSound = nullptr;

            // Return the sound id
            return sound.id;
        }

        int PlaySound2D(int id, float volume)
        {
            // Throw an error if no sound exists for this id
            if (m_Sounds.size() <= id)
                throw std::runtime_error("FMODAudioSystem Error: No sound found with this id");

            // Play the sound
            FMOD::Channel* pChannel{};
            m_pSystem->playSound(m_Sounds[id].pSound, nullptr, false, &pChannel);

            // Apply the given volume
            pChannel->setVolume(volume);

            // Retrieve the channel id
            int channel{};
            pChannel->getIndex(&channel);

            return channel;
        }

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

void leap::audio::FmodAudioSystem::LoadSoundAsync(const std::string& filePath, const std::function<void(int)>& callback)
{
}

void leap::audio::FmodAudioSystem::PlaySound2D(int id, float volume, const std::function<void(int)>& callback)
{
    // Delegate the play sound to the pImpl
    const int channel{ m_pImpl->PlaySound2D(id, volume) };

    // Invoke the callback
    if(callback) callback(channel);
}

void leap::audio::FmodAudioSystem::PlaySound3D(int id, const SoundData3D& soundData, const std::function<void(int)>& callback)
{
}

void leap::audio::FmodAudioSystem::SetVolume2D(int id, float volume)
{
}

void leap::audio::FmodAudioSystem::UpdateSound3D(int id, const SoundData3D& soundData)
{
}

void leap::audio::FmodAudioSystem::UpdateListener3D(const glm::vec3& position)
{
}

void leap::audio::FmodAudioSystem::Pause(int id)
{
}

void leap::audio::FmodAudioSystem::Resume(int id)
{
}

void leap::audio::FmodAudioSystem::PauseAll()
{
}

void leap::audio::FmodAudioSystem::ResumeAll()
{
}

void leap::audio::FmodAudioSystem::Mute(int id)
{
}

void leap::audio::FmodAudioSystem::Unmute(int id)
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
