#Audio engine cmake
add_library(AudioEngine "Interfaces/AudioSystem.h" "Interfaces/AudioSystem.cpp" "HelperObjects/SoundData3D.h" "Systems/FmodAudioSystem.h" "Systems/FmodAudioSystem.cpp")

set(AudioEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)