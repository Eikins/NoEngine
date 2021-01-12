# CMakeList.txt : CMake project for Core, include source and define
# project specific logic here.

find_package(Assimp CONFIG REQUIRED)

# Add source to this project's executable.
add_executable (NoEngine
	
	"NoEngine.h"
	"src/Transform.cpp"
	"src/Components/Camera.cpp"
	"src/Assets/Mesh.cpp" 
	"src/Components/Component.cpp" 
	"src/GameObject.cpp" 
	"src/Assets/Scene.cpp" 
	"src/Assets/Material.cpp"
	"src/Assets/Asset.cpp"
	"include/Core/Input/InputMaster.h"
	"include/Core/Layer.h"
	"src/Layer.cpp"
	"src/Components/Renderer.cpp"
	"src/Assets/Shader.cpp"
	"include/Core/Assets/Script.h"
	"include/Core/Components/ScriptedBehaviour.h"
	"src/Components/ScriptedBehaviour.cpp"
	"include/Utils/MemoryPools.hpp"
	"include/Core/Components/AABBCollider.h"
	"include/Core/Modules/ComponentContainer.hpp"
	"include/Core/Modules/ComponentManager.hpp"
	"include/Core/Modules/GameObjectManager.h"
	"src/Modules/GameObjectManager.cpp"
	"include/Core/Modules/GameManager.hpp"
	"src/Modules/GameManager.cpp"
	"include/Core/Systems/System.h"
	"include/Core/Modules/SystemManager.hpp"
	"src/Modules/SystemManager.cpp"
	"include/Core/Systems/PhysicsSystem.h" 
	"src/Systems/PhysicsSystem.cpp" 
	"include/Core/Systems/CollisionSystem.h" 
	"include/Core/Components/Collider.h" 
	"src/Systems/CollisionSystem.cpp" 
	"include/Core/Components/Rigidbody.h" 
	"include/Core/Systems/GraphicsSystem.h" 
	"src/Systems/GraphicsSystem.cpp" 
	"include/Core/Components/Light.h" 
	"src/Time.cpp"
	"src/Input/InputMaster.cpp" "include/Core/Systems/ScriptSystem.h" "src/Systems/ScriptSystem.cpp" "NoEngine.cpp")

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
