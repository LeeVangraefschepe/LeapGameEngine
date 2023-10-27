#Graphics engine cmake
add_library(GraphicsEngine STATIC
"DirectX/DirectXEngine.cpp"
"Camera.cpp"
"DirectX/DirectXMeshRenderer.cpp"
"DirectX/DirectXMaterial.cpp"
"Shaders/Pos3D.cpp"
"DirectX/DirectXShaderReader.cpp"
"ShaderDelete.cpp"
"Shaders/PosNorm3D.cpp"
"DirectX/DirectXMeshLoader.cpp"
"DirectX/DirectXTexture.cpp"
"Shaders/PosNormTex3D.cpp"
"DirectX/DirectXRenderTarget.cpp"
"DirectX/DirectXShadowRenderer.cpp"
"Shaders/ShadowMap.cpp"
"DirectionalLight.cpp"
"ImGui/imgui.cpp"
"ImGui/imgui_demo.cpp"
"ImGui/imgui_draw.cpp"
"ImGui/imgui_tables.cpp"
"ImGui/imgui_widgets.cpp"
"ImGui/imgui_impl_glfw.cpp"
"DirectX/imgui_impl_dx11.cpp"
"DirectX/DirectXSpriteRenderer.cpp"
"Shaders/Sprites.cpp" 
"DirectX/DirectXDefaults.cpp")

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

target_include_directories(
	GraphicsEngine PUBLIC 
	${GLFW_INCLUDE_DIR}
	${GLMIncludeDir}
	${EngineUtilsIncludeDir}
	${DIRECTX_TEX_INCLUDE_DIR}
	${FX11_INCLUDE_DIR}
)
target_link_libraries(GraphicsEngine PRIVATE ${GLFW_LIBRARY_DIR} ${FX11_LIBRARY_DIR})
target_link_libraries(GraphicsEngine PUBLIC EngineUtils)

message("Include dir: ${GraphicsEngineIncludeDir}")