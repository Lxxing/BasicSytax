cmake_minimum_required(VERSION 3.4.1)

# set the project name
project(ClassVirtual)


set(SRC_LIST OpenA.cpp Sytax.cpp)

MESSAGE(STATUS "This is BINARY dir " ${ClassVirtual_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir "${ClassVirtual_SOURCE_DIR})


# add the executable
add_executable(Tutorial ${SRC_LIST})


