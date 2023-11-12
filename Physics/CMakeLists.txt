#Physics engine cmake
add_library(PhysicsEngine STATIC
"PhysX/PhysXEngine.cpp"
"PhysX/PhysXScene.cpp" 
"PhysX/PhysXObject.cpp" 
"PhysX/PhysXShapes.cpp" 
"Data/Rigidbody.cpp" 
"PhysX/PhysXMaterial.cpp" 
"PhysX/PhysXSimulationCallbacks.cpp" 
"PhysX/PhysXSimulationFilterCallback.cpp")

set(PhysicsEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "")

target_include_directories(PhysicsEngine PUBLIC ${GLMIncludeDir} ${PHYSX_INCLUDE_DIR} ${EngineUtilsIncludeDir})
target_link_libraries(PhysicsEngine PUBLIC EngineUtils)
target_link_libraries(PhysicsEngine PRIVATE 
${PHYSX_LIBRARY_DIR} 
${PHYSX_COMMON_LIBRARY_DIR} 
${PHYSX_COOKING_LIBRARY_DIR} 
${PHYSX_FOUNDATION_LIBRARY_DIR} 
${PHYSX_LOWLEVELAABB_LIBRARY_DIR}
${PHYSX_LOWLEVELDYNAMICS_LIBRARY_DIR}
${PHYSX_LOWLEVEL_LIBRARY_DIR}
${PHYSX_CHARACTERKINEMATIC_LIBRARY_DIR}
${PHYSX_EXTENSIONS_LIBRARY_DIR}
${PHYSX_PVDSDK_LIBRARY_DIR}
${PHYSX_TASK_LIBRARY_DIR}
${PHYSX_VEHICLE_LIBRARY_DIR}
${PHYSX_SCENEQUERY_LIBRARY_DIR}
${PHYSX_SIMULATIONCONTROLLER_LIBRARY_DIR})