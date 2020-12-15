# CMakeList.txt : CMake project for Core, include source and define
# project specific logic here.

# Add source to this project's executable.
add_executable (NoEngine "NoEngine.cpp" "NoEngine.h"
	"src/Transform.cpp"
)

add_dependencies(NoEngine Math)
add_dependencies(NoEngine Graphics)
add_dependencies(NoEngine Scripting)
# Include Libs
target_include_directories(NoEngine PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include lib)
target_link_libraries(NoEngine Math Graphics Scripting)

install(TARGETS NoEngine RUNTIME DESTINATION bin)
