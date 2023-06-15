#Physics engine cmake
add_library(PhysicsEngine Physics.h Physics.cpp)

set(PhysicsEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)