# Bytecode cmake

add_library(Bytecode VM.h VM.cpp)

set(BytecodeIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)