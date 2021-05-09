# Locate the mono library
#
# This module defines the following variables:
#
# Mono_LIBRARY the name of the library;
# Mono_INCLUDE_DIR where to find mono include files.
# Mono_FOUND true if both the MONO_LIBRARY and MONO_INCLUDE_DIR have been found.
#
# To help locate the library and include file, you can define a
# variable called MONO_ROOT which points to the root of the mono library
# installation.
#
# default search dirs
# 
# Cmake file from Eikins

set( _mono_HEADER_SEARCH_DIRS
"/usr/include/mono-2.0/"
"/usr/local/include/mono-2.0/"
"${CMAKE_SOURCE_DIR}/includes/mono-2.0/"
"C:/Program Files (x86)/mono/include/mono-2.0/"
"C:/dev/mono/include/mono-2.0/" )
set( _mono_LIB_SEARCH_DIRS
"/usr/lib"
"/usr/local/lib"
"${CMAKE_SOURCE_DIR}/lib"
"C:/Program Files (x86)/mono/"
"C:/dev/mono/lib" )

# Check environment for root search directory
set( _mono_ENV_ROOT $ENV{MONO_ROOT} )
if( NOT MONO_ROOT AND _MONO_ENV_ROOT )
	set(MONO_ROOT ${_mono_ENV_ROOT} )
endif()

# Put user specified location at beginning of search
if( MONO_ROOT )
	list( INSERT _mono_HEADER_SEARCH_DIRS 0 "${MONO_ROOT}/include/mono-2.0/" )
	list( INSERT _mono_LIB_SEARCH_DIRS 0 "${MONO_ROOT}/lib" )
endif()

# Search for the header
FIND_PATH(Mono_INCLUDE_DIR "mono/jit/jit.h" PATHS ${_mono_HEADER_SEARCH_DIRS} )

# Search for the library
FIND_LIBRARY(Mono_LIBRARY NAMES mono-2.0-sgen
PATHS ${_mono_LIB_SEARCH_DIRS} )
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Mono DEFAULT_MSG Mono_LIBRARY Mono_INCLUDE_DIR)