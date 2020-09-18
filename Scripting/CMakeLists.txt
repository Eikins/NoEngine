# CMakeList.txt : CMake project for Scripting, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_library(Scripting STATIC src/Scripting.cpp)
target_include_directories(Scripting PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

install(TARGETS Scripting 
		LIBRARY DESTINATION lib 
		ARCHIVE DESTINATION lib)

# TODO: Add tests and install targets if needed.
