[![Build Status](https://github.com/LeeVangraefschepe/LeapGameEngine/actions/workflows/cmake.yml/badge.svg)](#)
[![GitHub Release](https://img.shields.io/github/v/release/LeeVangraefschepe/LeapGameEngine?logo=github&sort=semver)](#)
<a href="#"><img alt="ViewCount" src="https://views.whatilearened.today/views/github/LeeVangraefschepe/LeapGameEngine.svg"></a>

## Introduction
Leap Game Engine is an open-source 3D component-based C++20 game engine.  
Everyone is free to send us pull-requests with new features!

The engine is founded by two people:
[Lee Vangraefschepe](https://github.com/LeeVangraefschepe)
[Sander De Keukelaere](https://github.com/AtlantiaKing)

## Features
### Base Engine:
- Component/Gameobject based
- Transform component
- GameContext singleton with a timer class and a customizable window
- Canvas based UI system with sprites & buttons
- ServiceLocator which is the entry point to physics, rendering and audio
- Debug logging to console, ImGui & files

### Graphics:
- Obj loader
- Textures
- Materials & shaders
- Sprite rendering
- Camera
- Directional light
- Terrain

DirectX11 is the renderer library with the engine, using .fx files for shaders.  
The whole graphics engine is interfaced, giving the possibility for own implementations.

### Physics:
- Rigidbody (dynamic & kinematic)
- Colliders (box, sphere & capsule)
- Triggers
- Collision & trigger callbacks
- Physics materials
- Debug rendering

PhysX is the physics library included with the engine.  
The whole physics engine is interfaced, giving the possibility for own implementations.

### Utils:
The utils engine provides the engine and the user with some frequently used programming patterns.

- Command
- Observer
- Singleton

The utils engine also provides frequently needed code.

- Quaternion to Euler and vice-versa conversions
- Debugger


### Audio:
- AudioClips
- AudioSources
- AudioListener

FMOD is the audio library included with the engine.  
The whole audio engine is interfaced, giving the possibility for own implementations.

### Input:
The input engine is a command based input system.

There is currently support for:
- Keyboard
- Mouse

## Links
### Third party libraries
#### Rendering
[DirectX11](https://learn.microsoft.com/en-us/windows/win32/direct3d11/atoc-dx-graphics-direct3d-11)
[FX11](https://github.com/microsoft/FX11)
[GLFW](https://www.glfw.org/)

#### Audio
[FMOD](https://www.fmod.com/docs/2.02/api/welcome.html)

#### Math
[GLM](https://github.com/g-truc/glm)

#### Physics
[NVIDIA PhysX](https://www.nvidia.com/en-us/drivers/physx/physx-9-19-0218-driver/)

#### Utils
[Visual Leak Detector](https://kinddragon.github.io/vld/)
[Dear ImGui](https://www.dearimgui.com/)
