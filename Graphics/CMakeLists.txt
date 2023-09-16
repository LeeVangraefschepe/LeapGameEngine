#Graphics engine cmake
add_library(GraphicsEngine STATIC Renderer.h Renderer.cpp Vulkan/VulkanTypes.h Vulkan/VulkanInitializers.h Vulkan/VulkanInitializers.cpp Vulkan/VulkanEngine.h Vulkan/VulkanEngine.cpp Vulkan/VkBootstrap.h Vulkan/VkBootstrap.cpp)

set(GraphicsEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)