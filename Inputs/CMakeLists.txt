#Input engine cmake
add_library(InputEngine "InputManager.h" "InputManager.cpp" "Mouse.h" "Mouse.cpp" "Keyboard.h" "Keyboard.cpp")

set(InputEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)