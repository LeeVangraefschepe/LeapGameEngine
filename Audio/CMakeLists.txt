#Audio engine cmake
add_library(AudioEngine "Interfaces/AudioSystem.h"  "HelperObjects/SoundData3D.h" "Systems/FmodAudioSystem.h" "Systems/FmodAudioSystem.cpp" "HelperObjects/ChannelData.h")

set(AudioEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)