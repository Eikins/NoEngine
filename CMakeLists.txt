# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.9)

project ("NoEngine")

# Add our Cmake modules
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake/Modules")

# Set the C++ Version
message("!REQUIRED! -- Supported features = ${cxx_std_17}")
message("Supported features = ${cxx_std_17}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# Add the Editor support
add_definitions( -DNoEngine_Editor )

# Add the modules
add_subdirectory ("Math")
add_subdirectory ("Scripting")
add_subdirectory ("Graphics")
add_subdirectory ("Editor")
add_subdirectory ("Core")

add_custom_target(Update-Assets ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Assets ${CMAKE_BINARY_DIR}/Assets)