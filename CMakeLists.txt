# Root cmake
cmake_minimum_required(VERSION 3.22)

Project(Leap VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()

# OpenGL
find_package(OpenGL REQUIRED)

# Vulkan
find_package(Vulkan REQUIRED)

# Third party
add_subdirectory(3rdParty)

# GLFW include paths
set(GLFW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/GLFW/include")
set(GLFW_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/GLFW/lib/glfw3.lib")
message("Include dir: ${GLFW_INCLUDE_DIR} with ${GLFW_LIBRARY_DIR}")

# FMOD
set(FMOD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/FMOD/include")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(FMOD_DLL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/FMOD/bin/debug/fmodL64.dll")
	set(FMOD_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/FMOD/lib/debug/fmodL64_vc.lib")
else()
	set(FMOD_DLL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/FMOD/bin/release/fmod64.dll")
	set(FMOD_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/FMOD/lib/release/fmod64_vc.lib")
endif()
message("Including FMOD: ${FMOD_INCLUDE_DIR} with ${FMOD_LIBRARY_DIR} and ${FMOD_DLL_DIR}")

# PhysX
set(PHYSX_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PhysX/include")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	# DLLs
	set(PHYSX_DLL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PhysX/bin/debug/PhysX_64.dll")
	set(PHYSX_COMMON_DLL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PhysX/bin/debug/PhysXCommon_64.dll")
	set(PHYSX_COOKING_DLL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PhysX/bin/debug/PhysXCooking_64.dll")
	set(PHYSX_FOUNDATION_DLL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PhysX/bin/debug/PhysXFoundation_64.dll")
	# LIBs
	set(PHYSX_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PhysX/lib/debug/PhysX_64.lib")
	set(PHYSX_COMMON_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PhysX/lib/debug/PhysXCommon_64.lib")
	set(PHYSX_COOKING_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PhysX/lib/debug/PhysXCooking_64.lib")
	set(PHYSX_FOUNDATION_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PhysX/lib/debug/PhysXFoundation_64.lib")
else()
	# DLLs
	set(PHYSX_DLL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PhysX/bin/release/PhysX_64.dll")
	set(PHYSX_COMMON_DLL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PhysX/bin/release/PhysXCommon_64.dll")
	set(PHYSX_COOKING_DLL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PhysX/bin/release/PhysXCooking_64.dll")
	set(PHYSX_FOUNDATION_DLL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PhysX/bin/release/PhysXFoundation_64.dll")
	# LIBs
	set(PHYSX_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PhysX/lib/release/PhysX_64.lib")
	set(PHYSX_COMMON_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PhysX/lib/release/PhysXCommon_64.lib")
	set(PHYSX_COOKING_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PhysX/lib/release/PhysXCooking_64.lib")
	set(PHYSX_FOUNDATION_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/PhysX/lib/release/PhysXFoundation_64.lib")
endif()
message("Including PHYSX: ${PHYSX_INCLUDE_DIR}")

# Utils
add_subdirectory(Utils)
message("Including Utils: ${EngineUtilsIncludeDir}")

# Physics Engine
add_subdirectory(Physics)
message("Including Physics: ${PhysicsEngineIncludeDir}")

target_include_directories(
	PhysicsEngine PUBLIC 
	${PHYSX_INCLUDE_DIR}
	${EngineUtilsIncludeDir}
)
target_link_libraries(
	PhysicsEngine PUBLIC
	${PHYSX_LIBRARY_DIR}
	${PHYSX_COMMON_LIBRARY_DIR}
	${PHYSX_COOKING_LIBRARY_DIR}
	${PHYSX_FOUNDATION_LIBRARY_DIR}
	GLM
)

# Graphics Engine
add_subdirectory(Graphics)
message("Including Graphics: ${GraphicsEngineIncludeDir}")

target_include_directories(
	GraphicsEngine PUBLIC 
	${GLFW_INCLUDE_DIR}
	${EngineUtilsIncludeDir}
	${VkBootstrapIncludeDir}
)
target_link_libraries(
	GraphicsEngine PUBLIC 
	${GLFW_LIBRARY_DIR} 
	Vulkan::Vulkan 
	OpenGL::GL
	VMA
	GLM
	VkBootstrap
	tinyobjloader
)

# Input Engine
add_subdirectory(Inputs)
message("Including Input: ${InputEngineIncludeDir}")

target_include_directories(
	InputEngine PUBLIC 
	${GLFW_INCLUDE_DIR} 
	${EngineUtilsIncludeDir}
)
target_link_libraries(
	InputEngine PUBLIC 
	${GLFW_LIBRARY_DIR} 
	OpenGL::GL
	GLM
)

# Audio Engine
add_subdirectory(Audio)
message("Including Audio: ${AudioEngineIncludeDir}")

target_include_directories(
	AudioEngine PUBLIC 
	${FMOD_INCLUDE_DIR} 
	${EngineUtilsIncludeDir}
)
target_link_libraries(
	AudioEngine PUBLIC 
	${FMOD_LIBRARY_DIR} 
	GLM
)

# Networking Engine
add_subdirectory(Networking)
message("Including Networking: ${NetworkingEngineIncludeDir}")

target_include_directories(
	NetworkingEngine PUBLIC 
	${EngineUtilsIncludeDir}
)

# Main Engine
add_subdirectory(LeapEngine)
message("Including Engine: ${LeapEngineIncludeDir}")

target_include_directories(
	LeapEngine PUBLIC 
	${PhysicsEngineIncludeDir} 
	${GraphicsEngineIncludeDir}
	${InputEngineIncludeDir}
	${AudioEngineIncludeDir}
	${NetworkingEngineIncludeDir}
	${GLFW_INCLUDE_DIR}
	${EngineUtilsIncludeDir})
target_link_libraries(
	LeapEngine PUBLIC 
	PhysicsEngine
	GraphicsEngine
	InputEngine
	AudioEngine
	NetworkingEngine
	${GLFW_LIBRARY_DIR}
	OpenGL::GL
	GLM
)

# Game Project
add_subdirectory(UnnamedAdventureGame)
target_include_directories(
	UnnamedAdventureGame PUBLIC 
	${LeapEngineIncludeDir}
)
target_link_libraries(
	UnnamedAdventureGame PUBLIC 
	LeapEngine
)