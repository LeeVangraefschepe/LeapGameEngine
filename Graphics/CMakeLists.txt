#Graphics engine cmake
add_library(GraphicsEngine STATIC Renderer.h Renderer.cpp VulkanTypes.h VulkanInitializers.h VulkanInitializers.cpp VulkanEngine.h VulkanEngine.cpp VkBootstrap.h VkBootstrap.cpp)

set(GraphicsEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)