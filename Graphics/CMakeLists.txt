# CMakeList.txt : CMake project for Graphics, include source and define
# project specific logic here.

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Add source to this project's executable.
add_library(Graphics STATIC "../Scripting/VulkanImpl.cpp")

target_include_directories(Graphics PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include "${Vulkan_INCLUDE_DIRS}")
target_link_libraries(Graphics PUBLIC "${Vulkan_LIBRARIES}" glm glfw)

install(TARGETS Graphics 
		LIBRARY DESTINATION lib 
		ARCHIVE DESTINATION lib)

# TODO: Add tests and install targets if needed.
