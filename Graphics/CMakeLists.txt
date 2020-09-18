# CMakeList.txt : CMake project for Graphics, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

include_directories("${Vulkan_INCLUDE_DIRS}")
include_directories("${GLFW3_INCLUDE_DIR}")

# Add source to this project's executable.
add_library(Graphics STATIC src/VulkanApplication.cpp)
target_include_directories(Graphics PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(Graphics "${Vulkan_LIBRARIES}" "${GLFW3_LIBRARY}")

install(TARGETS Graphics 
		LIBRARY DESTINATION lib 
		ARCHIVE DESTINATION lib)

# TODO: Add tests and install targets if needed.
