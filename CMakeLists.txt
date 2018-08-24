cmake_minimum_required(VERSION 3.1)

project(three-dim-util CXX)

set(CMAKE_CXX_STANDARD 11)

if(APPLE AND EXISTS /usr/local/opt/qt)
	list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt")
endif()
if(APPLE AND EXISTS /usr/local/opt/qt5)
	list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
endif()

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Qt5 COMPONENTS Widgets Gui REQUIRED)
find_package(glm REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${Qt5Gui_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/include/three-dim-util/*.hpp)
file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)

add_library(three-dim-util STATIC ${headers} ${sources})
target_link_libraries(three-dim-util ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} Qt5::Gui Qt5::Widgets glm)
target_include_directories(three-dim-util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/three-dim-util)
install(TARGETS three-dim-util ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

option(THREEDIMUTIL_BUILD_DEMOS "Build demos" OFF)
if(THREEDIMUTIL_BUILD_DEMOS)
	add_subdirectory(demos/trackball-widget-demo)
endif()
