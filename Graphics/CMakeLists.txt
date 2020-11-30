# CMakeList.txt : CMake project for Graphics, include source and define
# project specific logic here.

find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)

find_package(glew CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Add source to this project's executable.
add_library(Graphics STATIC 
	"src/VulkanImpl.cpp"

#	"src/Vulkan/VKFramebuffer.cpp"
#	"src/Vulkan/VKPipelineState.cpp"
#	"src/Vulkan/VKRenderDevice.cpp"
#	"src/Vulkan/VKShader.cpp"
#	"src/Vulkan/VKSwapChain.cpp"
#	"src/Vulkan/VKWindow.cpp"

#	"src/OpenGL/GLRenderDevice.cpp"
#	"src/OpenGL/GLWindow.cpp"
#	"src/OpenGL/GLBuffer.cpp"
#	"src/OpenGL/GLShader.cpp"
#	"src/OpenGL/GLPipelineState.cpp"
#	"src/OpenGL/GLCommandBuffer.cpp"
)

target_include_directories(Graphics PUBLIC 
	${CMAKE_CURRENT_LIST_DIR}/include 
	${OPENGL_INCLUDE_DIR}
	${Vulkan_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIR}
)

target_link_libraries(Graphics PUBLIC 
	${OPENGL_LIBRARIES} 
	${Vulkan_LIBRARIES} 
	glm 
	glfw
	${GLEW_LIBRARY}
)

install(TARGETS Graphics 
		LIBRARY DESTINATION lib 
		ARCHIVE DESTINATION lib)

# TODO: Add tests and install targets if needed.
