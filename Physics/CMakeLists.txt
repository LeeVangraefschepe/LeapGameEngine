#Physics engine cmake
add_library(PhysicsEngine STATIC
Physics.cpp)

set(PhysicsEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

target_include_directories(PhysicsEngine PUBLIC ${GLMIncludeDir} ${PHYSX_INCLUDE_DIR} ${EngineUtilsIncludeDir})
target_link_libraries(PhysicsEngine PUBLIC EngineUtils)
target_link_libraries(PhysicsEngine PRIVATE ${PHYSX_LIBRARY_DIR} ${PHYSX_COMMON_LIBRARY_DIR} ${PHYSX_COOKING_LIBRARY_DIR} ${PHYSX_FOUNDATION_LIBRARY_DIR})
message("Include dir: ${PhysicsEngineIncludeDir}")