# CMakeList.txt : CMake project for Maths, include source and define
# project specific logic here.

# Add source to this project's executable.
add_library(Math STATIC 
	"src/Vector2.cpp"
	"src/Vector3.cpp"
	"src/Vector4.cpp"
	"src/Quaternion.cpp"
	"src/Rect.cpp"

	"src/Matrix4x4.cpp"
)

target_include_directories(Math PUBLIC 
	${CMAKE_CURRENT_LIST_DIR}/include 
)

target_link_libraries(Math PUBLIC 
)

install(TARGETS Math 
		LIBRARY DESTINATION lib 
		ARCHIVE DESTINATION lib)

# TODO: Add tests and install targets if needed.
