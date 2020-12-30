# CMakeList.txt : CMake project for Core, include source and define
# project specific logic here.

find_package(Assimp CONFIG REQUIRED)

# Add source to this project's executable.
add_executable (NoEngine
	"NoEngine.cpp"
	"NoEngine.h"
	"src/Transform.cpp"
	"src/Components/Camera.cpp"
	"src/Assets/Mesh.cpp" 
	"src/Components/Component.cpp" 
	"src/GameObject.cpp" 
	"src/Scene.cpp" 
	"src/Assets/Material.cpp"
	"src/Assets/Asset.cpp" "include/Core/Input/InputManager.h" "include/Core/Layer.h" "src/Layer.cpp" "src/Components/Renderer.cpp" "src/Assets/Shader.cpp" "include/Core/Assets/Script.h" "include/Core/Components/ScriptedBehaviour.h" "src/Components/ScriptedBehaviour.cpp")

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
