#Game cmake

add_executable(UnnamedAdventureGame main.cpp "Scenes/MainMenuScene.cpp")

# DLL Copies
# Define a custom target to copy the FMOD DLL
add_custom_command(TARGET UnnamedAdventureGame POST_BUILD
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
)