#Audio engine cmake
add_library(AudioEngine STATIC
	"FMOD/FmodAudioSystem.cpp" 
	"FMOD/FmodAudioClip.cpp"
)

target_include_directories(AudioEngine PRIVATE ${FMOD_INCLUDE_DIR})
target_include_directories(AudioEngine PUBLIC ${GLMIncludeDir} ${EngineUtilsIncludeDir})
target_link_libraries(AudioEngine PRIVATE ${FMOD_LIBRARY_DIR})
target_link_libraries(AudioEngine PUBLIC EngineUtils)

set(AudioEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)