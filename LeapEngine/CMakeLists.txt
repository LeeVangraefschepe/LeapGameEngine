#Leap engine library
add_library(LeapEngine "Leap.h" "Leap.cpp" "ServiceLocator.h" "ServiceLocator.cpp"  )

set(LeapEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)