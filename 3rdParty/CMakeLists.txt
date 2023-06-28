find_package(Vulkan REQUIRED)

#VMA
add_library(VMA INTERFACE)
target_include_directories(VMA INTERFACE VMA)

#GLM
add_library(GLM INTERFACE)
target_include_directories(GLM INTERFACE GLM)

#VkBootstrap
add_library(VkBootstrap STATIC)
target_sources(
	VkBootstrap PRIVATE
	vkbootstrap/VkBootstrap.h
	vkbootstrap/VkBootstrap.cpp
)

target_include_directories(VkBootstrap PUBLIC VkBootstrap)
target_link_libraries(VkBootstrap PUBLIC Vulkan::Vulkan)