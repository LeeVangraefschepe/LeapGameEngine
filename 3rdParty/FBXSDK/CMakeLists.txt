# FBXSDK CMake
set(FBX_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE PATH "")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(FBX_DLL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin/x64/debug/libfbxsdk.dll" CACHE PATH "")
	set(FBX_SDK_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/debug/libfbxsdk.lib" CACHE PATH "")
else()
	set(FBX_DLL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin/x64/release/libfbxsdk.dll" CACHE PATH "")
	set(FBX_SDK_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/release/libfbxsdk.lib" CACHE PATH "")
endif()