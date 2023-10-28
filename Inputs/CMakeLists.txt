#Input engine cmake
add_library(InputEngine STATIC
	"InputManager.cpp"
	"Mouse.cpp"
	"Keyboard.cpp"
)

target_include_directories(InputEngine PUBLIC ${GLFW_INCLUDE_DIR} ${GLMIncludeDir} ${EngineUtilsIncludeDir})
target_link_libraries(InputEngine PRIVATE ${GLFW_LIBRARY_DIR})
target_link_libraries(InputEngine PUBLIC EngineUtils)

set(InputEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)