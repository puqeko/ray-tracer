cmake_minimum_required(VERSION 2.6)
project(lab)

include_directories(
	/usr/include
	/usr/local/include
	./lib
)

if (APPLE)
	find_package(OpenGL)
	find_package(GLUT)
	include_directories(${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})
    set(CMAKE_CXX_FLAGS "-std=c++11 -Wno-deprecated")
else ()
    # use special flag to enable complex numbers on gnu machines
    set(CMAKE_CXX_FLAGS "-std=c++11 -Wno-deprecated -fext-numeric-literals")
endif ()

add_executable(main RayTracer.cpp)

file(GLOB SOURCE_FILES ./lib/*.cpp)
file(GLOB HEADER_FILES ./lib/*.hpp)
add_library(rtlibs
	${SOURCE_FILES}
	${HEADER_FILES}
)

if (APPLE)
	target_link_libraries(main
		${OPENGL_gl_LIBRARY}
		${OPENGL_glu_LIBRARY}
		${GLUT_LIBRARIES}
		rtlibs
	)
else ()
	target_link_libraries(main
		GL
		GLU
		glut
		rtlibs
	)
endif()

