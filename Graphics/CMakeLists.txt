#Graphics engine cmake
add_library(GraphicsEngine STATIC Renderer.h Renderer.cpp Vulkan/VulkanTypes.h Vulkan/VulkanInitializers.h Vulkan/VulkanInitializers.cpp Vulkan/VulkanEngine.h Vulkan/VulkanEngine.cpp Vulkan/VkBootstrap.h Vulkan/VkBootstrap.cpp DirectX/DirectXEngine.cpp Interfaces/IRenderer.h  Camera.h Camera.cpp  "DirectX/DirectXMeshRenderer.cpp" "DirectX/DirectXMaterial.cpp" "Data/Vertex.h" "Interfaces/IMaterial.h" "Shader.h" "Shader.cpp" "Shaders/Pos3D.cpp" "DirectX/DirectXShaderReader.h" "DirectX/DirectXShaderReader.cpp" "ShaderDelete.cpp" "Utils.h" "Shaders/PosNorm3D.h" "Shaders/PosNorm3D.cpp" "DirectX/DirectXMeshLoader.h" "DirectX/DirectXMeshLoader.cpp"  "Interfaces/ITexture.h" "DirectX/DirectXTexture.h" "DirectX/DirectXTexture.cpp" "Shaders/PosNormTex3D.h" "Shaders/PosNormTex3D.cpp" "Data/CustomMesh.h" "DirectX/DirectXRenderTarget.h" "DirectX/DirectXRenderTarget.cpp")

# DirectXTex
set(DIRECTX_TEX_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/DirectXTex/include")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(DIRECTX_TEX_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/DirectXTex/lib/debug/DirectXTex.lib")
else()
	set(DIRECTX_TEX_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/DirectXTex/lib/release/DirectXTex.lib")
endif()

# FX11
set(FX11_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/FX11/include")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
set(FX11_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/FX11/lib/debug/Effects11.lib")
else()
set(FX11_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty/FX11/lib/release/Effects11.lib")
endif()

set(GraphicsEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

target_include_directories(GraphicsEngine PUBLIC ${DIRECTX_TEX_INCLUDE_DIR} ${FX11_INCLUDE_DIR})
target_link_libraries(GraphicsEngine PUBLIC ${FX11_LIBRARY_DIR})