cmake_minimum_required(VERSION 3.1)

project(three-dim-util CXX)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})

file(GLOB headers three-dim-util/*.hpp)
file(GLOB sources three-dim-util/*.cpp)

add_library(three-dim-util STATIC ${headers} ${sources})
target_link_libraries(three-dim-util ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/three-dim-util)
install(TARGETS three-dim-util ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
