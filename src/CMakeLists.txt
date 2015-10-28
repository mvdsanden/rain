cmake_minimum_required (VERSION 2.6)
project (rain)

# The version number.
set (rain_VERSION_MAJOR 0)
set (rain_VERSION_MINOR 1)

set (CMAKE_CXX_FLAGS "-std=c++11 -pthread -ggdb")

set(THREADS_PREFER_PTHREAD_FLAG ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/config.hh.in"
  "${PROJECT_BINARY_DIR}/config.hh"
  )
 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# should we use our own math functions?
#option (USE_MYMATH 
#        "Use tutorial provided math implementation" ON) 

include_directories ("${PROJECT_SOURCE_DIR}")

# io
add_subdirectory (io)
add_subdirectory (core)
add_subdirectory (exceptions)
add_subdirectory (test)

#add_executable(Tutorial tutorial.cxx)
#target_link_libraries (Tutorial MathFunctions)


# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE  
     "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_VERSION_MAJOR "${rain_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${rain_VERSION_MINOR}")
include (CPack)
