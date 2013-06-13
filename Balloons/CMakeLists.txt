cmake_minimum_required(VERSION 2.6)
project(ambientLighting)

#There are lots of scripts with cmake
#for finding external libraries. 
#see /usr/local/share/cmake-2.6/Modules/Find*.cmake for more examples
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "-g -Wall")

set(CORELIBS ${GLUT_LIBRARY} ${OPENGL_LIBRARY})

add_executable(ambientLighting main.cpp render.cpp)
target_link_libraries(ambientLighting ${CORELIBS})
