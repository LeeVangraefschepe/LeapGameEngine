#Audio engine cmake
add_library(AudioEngine "Interfaces/AudioSystem.h" "Interfaces/AudioSystem.cpp" "HelperObjects/SoundData3D.h")

set(AudioEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)