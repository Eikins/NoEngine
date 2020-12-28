# CMakeList.txt : CMake project for Core, include source and define
# project specific logic here.

find_package(Assimp CONFIG REQUIRED)

# Add source to this project's executable.
add_executable (NoEngine
	"NoEngine.cpp"
	"NoEngine.h"
	"src/Transform.cpp"
	"src/Components/Camera.cpp"
 "include/Core/Mesh.h" "src/Mesh.cpp" "include/Core/Shader.h" "include/Core/Material.h" "include/Core/Components/Component.h" "src/Components/Component.cpp")

add_dependencies(NoEngine Math)
add_dependencies(NoEngine Graphics)
add_dependencies(NoEngine Scripting)
# Include Libs
target_include_directories(NoEngine PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/include
	lib
	${ASSIMP_INCLUDE_DIRS}
)

target_link_libraries(NoEngine
	Math
	Graphics
	Scripting 
	assimp::assimp
)

install(TARGETS NoEngine RUNTIME DESTINATION bin)
