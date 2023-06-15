# Root cmake
cmake_minimum_required(VERSION 3.22)

Project(Leap VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW include paths
set(GLFW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/GLFW/include")
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64-bit platform (x64)
	message("Building for x64")
	set(GLFW_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/GLFW/lib/x64/glfw3.lib")
else()
    # 32-bit platform (x86)
	message("Building for x86")
	set(GLFW_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/GLFW/lib/x86/glfw3.lib")
endif()
message("Include dir: ${GLFW_INCLUDE_DIR} with ${GLFW_LIBRARY_DIR}")

# GLM
add_subdirectory(3rdParty/GLM)
message("Include dir: ${GLMIncludeDir}")

# Physics Engine
add_subdirectory(Physics)
message("Include dir: ${PhysicsEngineIncludeDir}")

# Graphics Engine
add_subdirectory(Graphics)
message("Include dir: ${GraphicsEngineIncludeDir}")
target_include_directories(GraphicsEngine PUBLIC ${GLFW_INCLUDE_DIR} ${GLMIncludeDir})
target_link_libraries(GraphicsEngine PUBLIC ${GLFW_LIBRARY_DIR} OpenGL::GL)

# Input Engine
add_subdirectory(Inputs)
message("Include dir: ${InputEngineIncludeDir}")
target_include_directories(InputEngine PUBLIC ${GLFW_INCLUDE_DIR} ${GLMIncludeDir})
target_link_libraries(InputEngine PUBLIC ${GLFW_LIBRARY_DIR} OpenGL::GL)

# Audio Engine
add_subdirectory(Audio)
message("Include dir: ${AudioEngineIncludeDir}")
target_include_directories(GraphicsEngine PUBLIC ${GLMIncludeDir})

# Networking Engine
add_subdirectory(Networking)
message("Include dir: ${NetworkingEngineIncludeDir}")

# Main Engine
add_subdirectory(LeapEngine)
message("Include dir: ${LeapEngineIncludeDir}")
target_include_directories(
	LeapEngine PUBLIC 
	${PhysicsEngineIncludeDir} 
	${GraphicsEngineIncludeDir}
	${InputEngineIncludeDir}
	${AudioEngineIncludeDir}
	${NetworkingEngineIncludeDir}
	${GLFW_INCLUDE_DIR}
	${GLMIncludeDir})
target_link_libraries(
	LeapEngine PUBLIC 
	PhysicsEngine
	GraphicsEngine
	InputEngine
	AudioEngine
	NetworkingEngine
	${GLFW_LIBRARY_DIR}
	OpenGL::GL)

# Game Project
add_subdirectory(UnnamedAdventureGame)
target_include_directories(UnnamedAdventureGame PUBLIC ${LeapEngineIncludeDir})
target_link_libraries(UnnamedAdventureGame PUBLIC LeapEngine)