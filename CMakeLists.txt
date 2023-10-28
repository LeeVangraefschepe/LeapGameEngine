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

# Copy files
set(DATA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Data")
set(DESTINATION_COPY "${CMAKE_BINARY_DIR}/UnnamedAdventureGame/Data")

# 3rdParty
add_subdirectory(3rdParty)

# Utils
add_subdirectory(Utils)

# Physics Engine
add_subdirectory(Physics)

# Graphics Engine
add_subdirectory(Graphics)

# Input Engine
add_subdirectory(Inputs)

# Audio Engine
add_subdirectory(Audio)

# Networking Engine
add_subdirectory(Networking)

# Main Engine
add_subdirectory(LeapEngine)

# Game Project
add_subdirectory(UnnamedAdventureGame)