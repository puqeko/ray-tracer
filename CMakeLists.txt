cmake_minimum_required(VERSION 2.6)
project(lab)

include_directories(
	/usr/include
	/usr/local/include
)

if (APPLE)
	find_package(OpenGL)
	find_package(GLUT)
	include_directories(${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})
endif ()

set(CMAKE_CXX_FLAGS "-std=c++11 -Wno-deprecated")

add_executable(coil Coil.cpp)

if (APPLE)
	target_link_libraries(coil
		${OPENGL_gl_LIBRARY}
		${OPENGL_glu_LIBRARY}
		${GLUT_LIBRARIES}
	)
else ()
	target_link_libraries(coil
		GL
		GLU
		glut
	)
endif()

