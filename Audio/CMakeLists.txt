#Audio engine cmake
add_library(AudioEngine AudioSystem.h AudioSystem.cpp)

set(AudioEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)