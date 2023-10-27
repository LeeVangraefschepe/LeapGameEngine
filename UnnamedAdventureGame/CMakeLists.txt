# Game cmake

add_executable(UnnamedAdventureGame
"main.cpp"
"Scenes/MainMenuScene.cpp"
"Components/Transformator.cpp"
"Components/InfoUI.cpp"
"Components/FreeCamMovement.cpp"
"Components/WindowManager.cpp"
"Components/AudioTester.cpp")

target_include_directories(
	LeapEngine PUBLIC 
	${PhysicsEngineIncludeDir} 
	${GraphicsEngineIncludeDir}
	${InputEngineIncludeDir}
	${AudioEngineIncludeDir}
	${NetworkingEngineIncludeDir}
	${GLFW_INCLUDE_DIR}
	${GLMIncludeDir} 
	${EngineUtilsIncludeDir})
target_link_libraries(
	LeapEngine PUBLIC 
	PhysicsEngine
	GraphicsEngine
	InputEngine
	AudioEngine
	NetworkingEngine
	EngineUtils)
target_include_directories(UnnamedAdventureGame PUBLIC ${LeapEngineIncludeDir})
target_link_libraries(UnnamedAdventureGame PUBLIC LeapEngine ${GLFW_LIBRARY_DIR})

# Copies used for .exe
add_custom_command(TARGET UnnamedAdventureGame PRE_LINK
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FMOD_DLL_DIR} $<TARGET_FILE_DIR:UnnamedAdventureGame>
    COMMENT "Copying FMOD DLL..."
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PHYSX_DLL_DIR} $<TARGET_FILE_DIR:UnnamedAdventureGame>
    COMMENT "Copying PHYSX DLL..."
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PHYSX_COMMON_DLL_DIR} $<TARGET_FILE_DIR:UnnamedAdventureGame>
    COMMENT "Copying PHYSX COMMON DLL..."
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PHYSX_COOKING_DLL_DIR} $<TARGET_FILE_DIR:UnnamedAdventureGame>
    COMMENT "Copying PHYSX COOKING DLL..."
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PHYSX_FOUNDATION_DLL_DIR} $<TARGET_FILE_DIR:UnnamedAdventureGame>
    COMMENT "Copying PHYSX FOUNDATION DLL..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory  ${DATA_FILES} ${DESTINATION_COPY}
    COMMENT "Copying DATA..."
)