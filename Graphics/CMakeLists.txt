#Graphics engine cmake
add_library(GraphicsEngine STATIC Renderer.h Renderer.cpp Vulkan/VulkanTypes.h Vulkan/VulkanInitializers.h Vulkan/VulkanInitializers.cpp Vulkan/VulkanEngine.h Vulkan/VulkanEngine.cpp Vulkan/VkBootstrap.h Vulkan/VkBootstrap.cpp DirectX/DirectXEngine.cpp Interfaces/IRenderer.h Camera.h Camera.cpp)

# DirectXTex
set(DIRECTX_TEX_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/DirectXTex/include")
set(DIRECTX_TEX_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/DirectXTex/lib/release/DirectXTex.lib")

# FX11
set(FX11_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/FX11/include")
set(FX11_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/FX11/lib/release/Effects11.lib")

set(GraphicsEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

target_include_directories(GraphicsEngine PUBLIC ${DIRECTX_TEX_INCLUDE_DIR} ${FX11_INCLUDE_DIR})
target_link_libraries(GraphicsEngine PUBLIC ${FX11_LIBRARY_DIR})