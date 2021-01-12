# CMakeList.txt : CMake project for Scripting, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_library(Scripting STATIC "src/Runtime.cpp" "include/Scripting/Environment.h" "src/Environment.cpp" "include/Scripting/ScriptInstance.h" "src/ScriptInstance.cpp")

target_include_directories(Scripting PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include 
	${CMAKE_CURRENT_LIST_DIR}/../Core/include
	${CMAKE_CURRENT_LIST_DIR}/../Math/include
	${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/mono-2.0
)

target_link_directories(Scripting PUBLIC
	${MONO_LIBRARY_PATH}
)

target_link_libraries(Scripting PUBLIC mono-2.0-sgen)

add_custom_command(TARGET Scripting POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy "${MONO_BINARY_PATH}/mono-2.0-sgen.dll" "${CMAKE_BINARY_DIR}/bin/mono-2.0-sgen.dll"
	COMMENT "Copying mono-2.0-sgen.dll from ' ${MONO_LIBRARY_PATH}' to '${CMAKE_BINARY_DIR}/bin'" VERBATIM
)

install(TARGETS Scripting 
		LIBRARY DESTINATION lib 
		ARCHIVE DESTINATION lib)

# TODO: Add tests and install targets if needed.
