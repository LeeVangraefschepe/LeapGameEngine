# Utils cmake

add_library(EngineUtils "Debug.cpp" "Quaternion.cpp" "EventPool.h")

set(EngineUtilsIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

target_include_directories(EngineUtils PUBLIC ${GLMIncludeDir})