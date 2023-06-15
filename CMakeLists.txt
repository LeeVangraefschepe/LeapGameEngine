#Root cmake
cmake_minimum_required(VERSION 3.22)

Project(Leap VERSION 1.0.0)

add_subdirectory(LeapEngine)
message("Include dir: ${LeapEngineIncludeDir}")

add_subdirectory(UnnamedAdventureGame)
target_include_directories(UnnamedAdventureGame PUBLIC ${LeapEngineIncludeDir})

target_link_libraries(UnnamedAdventureGame PUBLIC LeapEngine)