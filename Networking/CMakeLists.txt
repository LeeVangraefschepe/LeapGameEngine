#Networking engine cmake
add_library(NetworkingEngine Client.h Client.cpp)

set(NetworkingEngineIncludeDir "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)