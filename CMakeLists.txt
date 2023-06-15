#Root cmake
cmake_minimum_required(VERSION 3.22)

Project(Leap VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64-bit platform (x64)
	message("Building for x64")
	set(GLFW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/GLFW/x64/include")
	set(GLFW_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/GLFW/x64/lib/glfw3.lib")
else()
    # 32-bit platform (x86)
	message("Building for x86")
	set(GLFW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/GLFW/x86/include")
	set(GLFW_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/GLFW/x86/lib/glfw3.lib")
endif()

add_subdirectory(Physics)
message("Include dir: ${PhysicsEngineIncludeDir}")

add_subdirectory(Graphics)
message("Include dir: ${GraphicsEngineIncludeDir}")

add_subdirectory(Inputs)
message("Include dir: ${InputEngineIncludeDir}")

add_subdirectory(Audio)
message("Include dir: ${AudioEngineIncludeDir}")

add_subdirectory(Networking)
message("Include dir: ${NetworkingEngineIncludeDir}")

add_subdirectory(LeapEngine)
message("Include dir: ${LeapEngineIncludeDir}")
target_include_directories(
	LeapEngine PUBLIC 
	${PhysicsEngineIncludeDir} 
	${GraphicsEngineIncludeDir}
	${InputEngineIncludeDir}
	${AudioEngineIncludeDir}
	${NetworkingEngineIncludeDir})
target_link_libraries(
	LeapEngine PUBLIC 
	PhysicsEngine
	GraphicsEngine
	InputEngine
	AudioEngine
	NetworkingEngine)

add_subdirectory(UnnamedAdventureGame)
target_include_directories(UnnamedAdventureGame PUBLIC ${LeapEngineIncludeDir})
target_link_libraries(UnnamedAdventureGame PUBLIC LeapEngine)