#Leap engine library
add_library(LeapEngine "Leap.h" "Leap.cpp" "GameContext/Timer.h" "GameContext/Timer.cpp" "GameContext/GameContext.h" "GameContext/GameContext.cpp" "ServiceLocator/ServiceLocator.h" "ServiceLocator/ServiceLocator.cpp" "SceneGraph/GameObject.h" "SceneGraph/GameObject.cpp" "Components/Component.h" "Components/Component.cpp"   "SceneGraph/Scene.h" "SceneGraph/Scene.cpp" "SceneGraph/SceneManager.h" "SceneGraph/SceneManager.cpp")

set(LeapEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)