# Graphics engine
add_library(GraphicsEngine STATIC Renderer.h Renderer.cpp VulkanTypes.h VulkanInitializers.h VulkanInitializers.cpp VulkanEngine.h VulkanEngine.cpp VkBootstrap.h VkBootstrap.cpp "VulkanPipelineBuilder.h" "VulkanPipelineBuilder.cpp")
set(GraphicsEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

## Shader compilation
# GLSL
find_program(
	GLSL_VALIDATOR glslangValidator 
	HINTS 
	/usr/bin
	/usr/local/bin
	$ENV{VULKAN_SDK}/Bin/
	$ENV{VULKAN_SDK}/Bin32/
)

# Shader files
file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.comp"
    )

# iterate each shader
foreach(GLSL ${GLSL_SOURCE_FILES})
  message(STATUS "BUILDING SHADER")
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Compiled/${FILE_NAME}.spv")
  message(STATUS ${GLSL})
  # execute glslang command to compile that specific shader
  add_custom_command(
    OUTPUT ${SPIRV}
	COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Compiled/"
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders 
    DEPENDS ${SPIRV_BINARY_FILES}
    )

add_dependencies(GraphicsEngine Shaders)

# add shaders to output dir
add_custom_command(TARGET GraphicsEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:GraphicsEngine>/Shaders/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Compiled/"
        "$<TARGET_FILE_DIR:GraphicsEngine>/Shaders"
        )