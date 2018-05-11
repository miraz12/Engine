#--------------------------------------------------------------------------
# example project
#--------------------------------------------------------------------------

PROJECT(example)
FILE(GLOB project_headers code/*.h)
FILE(GLOB project_sources code/*.cc)

SET(files_project ${project_headers} ${project_sources} )
SOURCE_GROUP("example" FILES ${files_project})

ADD_EXECUTABLE(example ${files_project})
TARGET_LINK_LIBRARIES(example core render)
ADD_DEPENDENCIES(example core render)
IF(MSVC)
	SET_TARGET_PROPERTIES(example PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
ENDIF()