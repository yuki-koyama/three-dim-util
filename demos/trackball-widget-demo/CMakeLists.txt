file(GLOB files main.cpp)
add_executable(trackball-widget-demo ${files})
target_link_libraries(trackball-widget-demo three-dim-util)
