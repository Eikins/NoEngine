# CMakeList.txt : CMake project for Graphics, include source and define
# project specific logic here.

find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)

find_package(glew CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Add source to this project's executable.
add_library(Graphics STATIC 
	"src/GraphicsContext.cpp"
	"src/Vulkan/VulkanBuffer.cpp"
	"src/Vulkan/VulkanContext.cpp"
	"src/Vulkan/VulkanSwapchain.cpp"
	"src/Vulkan/VulkanWindow.cpp"

	"src/Vulkan/VulkanRenderer.cpp"

	"src/imgui/imgui_impl_glfw.cpp"
	"src/Vulkan/VulkanImGui.cpp"
 "include/Graphics/Vulkan/VulkanMeshRenderer.h" "src/Vulkan/VulkanMeshRenderer.cpp")

add_dependencies(Graphics Math)
add_dependencies(Graphics Editor)

target_include_directories(Graphics PUBLIC 
	${CMAKE_CURRENT_LIST_DIR}/include 
	${OPENGL_INCLUDE_DIR}
	${Vulkan_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIR}

	${CMAKE_CURRENT_LIST_DIR}/../Math/include 
	${CMAKE_CURRENT_LIST_DIR}/../Core/include 
)

target_link_libraries(Graphics PUBLIC 
	${OPENGL_LIBRARIES} 
	${Vulkan_LIBRARIES} 
	glm 
	glfw
	${GLEW_LIBRARY}
	Math
	Editor
)

install(TARGETS Graphics 
		LIBRARY DESTINATION lib 
		ARCHIVE DESTINATION lib)

# TODO: Add tests and install targets if needed.
