# CMakeList.txt : CMake project for Graphics, include source and define
# project specific logic here.


# Add source to this project's executable.
add_library(Editor STATIC 
	"src/imgui/imgui.cpp"
	"src/imgui/imgui_demo.cpp"
	"src/imgui/imgui_draw.cpp"
	"src/imgui/imgui_tables.cpp"
	"src/imgui/imgui_widgets.cpp"

	"src/Editor/Editor.cpp"
 "include/Editor/GameObjectEditor.hpp" "include/Editor/SceneEditors.h" "src/Editor/SceneEditors.cpp")

target_include_directories(Editor PUBLIC 
	${CMAKE_CURRENT_LIST_DIR}/include
	${CMAKE_CURRENT_LIST_DIR}/include/imgui

	${CMAKE_CURRENT_LIST_DIR}/../Math/include
	${CMAKE_CURRENT_LIST_DIR}/../Core/include
)

target_link_libraries(Editor PUBLIC 
	
)

install(TARGETS Editor 
		LIBRARY DESTINATION lib 
		ARCHIVE DESTINATION lib)

# TODO: Add tests and install targets if needed.
