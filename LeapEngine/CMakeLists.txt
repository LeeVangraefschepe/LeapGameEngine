#Leap engine library
add_library(LeapEngine "Leap.h" "Leap.cpp" "GameContext/Timer.h" "GameContext/Timer.cpp" "GameContext/GameContext.h" "GameContext/GameContext.cpp" "ServiceLocator/ServiceLocator.h" "ServiceLocator/ServiceLocator.cpp")

set(LeapEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)