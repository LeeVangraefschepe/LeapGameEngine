# Utils cmake

add_library(EngineUtils
	"Debug.cpp"
	"Quaternion.cpp")

set(EngineUtilsIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

target_include_directories(EngineUtils PUBLIC ${GLMIncludeDir})