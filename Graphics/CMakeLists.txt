#Graphics engine cmake
add_library(GraphicsEngine Renderer.h Renderer.cpp)

set(GraphicsEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)