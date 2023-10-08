#Leap engine library
add_library(LeapEngine 
    "Leap.h"
    "Leap.cpp"
    "GameContext/Timer.h"
    "GameContext/Timer.cpp"
    "GameContext/GameContext.h"
    "GameContext/GameContext.cpp"
    "ServiceLocator/ServiceLocator.h"
    "ServiceLocator/ServiceLocator.cpp"
    "SceneGraph/GameObject.h"
    "SceneGraph/GameObject.cpp"
    "Components/Component.h"
    "Components/Component.cpp"
    "Components/Transform/Transform.h"
    "Components/Transform/Transform.cpp"
    "SceneGraph/Scene.h"
    "SceneGraph/Scene.cpp"
    "SceneGraph/SceneManager.h"
    "SceneGraph/SceneManager.cpp"
 "Components/RenderComponents/CameraComponent.h" "Components/RenderComponents/CameraComponent.cpp" "Components/RenderComponents/MeshRenderer.h" "Components/RenderComponents/MeshRenderer.cpp" "Components/RenderComponents/DirectionalLightComponent.cpp" "GameContext/Window.h" "GameContext/Window.cpp" "Components/RenderComponents/SpriteRendererComponent.h" "Components/RenderComponents/SpriteRendererComponent.cpp")

set(LeapEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)