cmake_minimum_required(VERSION 3.1)

project(three-dim-util CXX)

set(CMAKE_CXX_STANDARD 11)

if(APPLE AND EXISTS /usr/local/opt/qt)
	list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt")
endif()
if(APPLE AND EXISTS /usr/local/opt/qt5)
	list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
endif()

find_package(Eigen3 REQUIRED)
if((NOT TARGET Eigen3::Eigen) AND (DEFINED EIGEN3_INCLUDE_DIR))
    add_library(AliasEigen3 INTERFACE)
    target_include_directories(AliasEigen3 INTERFACE ${EIGEN3_INCLUDE_DIR})
    add_library(Eigen3::Eigen ALIAS AliasEigen3)
endif()
find_package(Qt5 COMPONENTS Widgets Gui REQUIRED)
find_package(glm REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/include/three-dim-util/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/three-dim-util/opengl2/primitives/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/three-dim-util/opengl2/widgets/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/three-dim-util/opengl2/*.hpp)
file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/opengl2/*.cpp)

add_library(three-dim-util STATIC ${headers} ${sources})
target_link_libraries(three-dim-util Eigen3::Eigen glm ${OPENGL_LIBRARIES} Qt5::Gui Qt5::Widgets)
target_include_directories(three-dim-util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include FILES_MATCHING PATTERN "*.h*")
install(TARGETS three-dim-util ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

option(THREEDIMUTIL_BUILD_DEMOS "Build demos" OFF)
if(THREEDIMUTIL_BUILD_DEMOS)
	add_subdirectory(demos/trackball-widget-demo)
endif()
