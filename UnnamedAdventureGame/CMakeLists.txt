#Game cmake

add_executable(UnnamedAdventureGame main.cpp)

# DLL Copies
# Define a custom target to copy the FMOD DLL
add_custom_command(TARGET UnnamedAdventureGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FMOD_DLL_DIR} $<TARGET_FILE_DIR:UnnamedAdventureGame>
    COMMENT "Copying FMOD DLL..."
)