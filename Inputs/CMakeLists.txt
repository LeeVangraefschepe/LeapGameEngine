#Input engine cmake
add_library(InputEngine "InputManager.h" "InputManager.cpp" "InputManagerGLFW3.h" "InputManagerGLFW3.cpp")

set(InputEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)