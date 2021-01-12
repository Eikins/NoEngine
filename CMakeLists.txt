# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.9)

# =================== Set user specific variables here ======================
# TODO : try to find a better approach (using env variables)

# vcpkg tooclchain
set(CMAKE_TOOLCHAIN_FILE "D:/Dev/Tools/vcpkg/scripts/buildsystems/vcpkg.cmake")

# MoONO
# Library path
set(MONO_LIBRARY_PATH "D:/Dev/SDKs/Mono/msvc/build/sgen/x64/lib/Release")
set(MONO_BINARY_PATH "D:/Dev/SDKs/Mono/msvc/build/sgen/x64/bin/Release")
# Runtime location
set(MONO_INSTALL_FOLDER "C:/Program\ Files\ (x86)/Mono")
#============================================================================

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
add_compile_definitions(MONO_LIB_DIR="${MONO_INSTALL_FOLDER}/lib")
add_compile_definitions(MONO_ETC_DIR="${MONO_INSTALL_FOLDER}/etc")
add_compile_definitions(MONO_BIN_DIR="${MONO_INSTALL_FOLDER}/bin")
add_compile_definitions(NoEngine_Editor)

# Add the modules
add_subdirectory ("Math")
add_subdirectory ("Scripting")
add_subdirectory ("Graphics")
add_subdirectory ("Editor")
add_subdirectory ("Core")

add_custom_target(Update-Assets ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Assets ${CMAKE_BINARY_DIR}/Assets)