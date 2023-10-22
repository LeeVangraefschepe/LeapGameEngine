#Audio engine cmake
add_library(AudioEngine STATIC
	"Interfaces/IAudioSystem.h"  
	"HelperObjects/SoundData3D.h" 
	"FMOD/FmodAudioSystem.h" 
	"FMOD/FmodAudioSystem.cpp" 
	"HelperObjects/ChannelData.h" 
	"Interfaces/IAudioClip.h" 
	"FMOD/FmodAudioClip.h"
	"FMOD/FmodAudioClip.cpp")

set(AudioEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)