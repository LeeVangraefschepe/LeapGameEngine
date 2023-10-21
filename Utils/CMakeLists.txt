# Utils cmake

add_library(EngineUtils "Debug.cpp")

set(EngineUtilsIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

add_subdirectory(Bytecode)
message("Include dir: ${BytecodeIncludeDir}")