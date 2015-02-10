# require an up to date version of CMAKE
cmake_minimum_required (VERSION 2.8)

# name your project whatever you want, here we call it TEST_BUILD

project (TEST_BUILD)

# set the standard flags
add_definitions("-Wall")

# enable this for optimization
#add_definitions("-O3")

# enable this for C++ 11
add_definitions(-std=c++0x)

# add the source code 
set (SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
file (GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cc")
include_directories ("${SRC_DIR}")

# add gtest
#set (GTEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/googletest")
#add_subdirectory (${GTEST_DIR})
find_package (gtest REQUIRED)

# build an executable called code
add_executable(code ${SRC_FILES})

# link it with gtest
target_link_libraries(code "gtest" "gtest_main")