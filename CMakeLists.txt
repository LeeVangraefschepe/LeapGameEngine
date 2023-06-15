#Root cmake
cmake_minimum_required(VERSION 3.22)

Project(Leap VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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