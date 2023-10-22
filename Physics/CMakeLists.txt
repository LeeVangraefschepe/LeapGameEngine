#Physics engine cmake
add_library(PhysicsEngine STATIC Physics.h Physics.cpp)

set(PhysicsEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)