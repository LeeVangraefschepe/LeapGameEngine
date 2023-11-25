#Networking engine cmake
add_library(NetworkingEngine STATIC
	"Leap/LeapPacket.cpp"
	"Leap/LeapClient.cpp")

target_include_directories(NetworkingEngine PUBLIC ${EngineUtilsIncludeDir} EngineUtils)

set(NetworkingEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)