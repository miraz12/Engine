project (libnoise)

set (NOISE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
find_package (OpenGL REQUIRED)
set (NOISE_LIBRARIES ${OPENGL_LIBRARIES})
include_directories (${NOISE_DIR}/include)

FILE(GLOB noise_headers ${NOISE_DIR}/*.h)

set(NOISE_HEAD 
	${NOISE_DIR} ${noise_headers}
)



add_definitions(-DGLEW_STATIC)
add_library (libnoise STATIC ${NOISE_HEAD})
#target_link_libraries (glew ${GLEW_LIBRARIES})