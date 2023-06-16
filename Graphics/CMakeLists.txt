#Graphics engine cmake
add_library(GraphicsEngine Renderer.h Renderer.cpp VulkanTypes.h VulkanInitializers.h VulkanInitializers.cpp VulkanEngine.h VulkanEngine.cpp)
set(GraphicsEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)