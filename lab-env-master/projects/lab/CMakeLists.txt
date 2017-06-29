#--------------------------------------------------------------------------
# lab project
#--------------------------------------------------------------------------

PROJECT(lab)
FILE(GLOB lab_headers code/*.h)
FILE(GLOB lab_sources code/*.cc)

SET(files_lab ${lab_headers} ${lab_sources} )
SOURCE_GROUP("lab" FILES ${files_lab})

ADD_EXECUTABLE(lab ${files_lab})
TARGET_LINK_LIBRARIES(lab core render)
ADD_DEPENDENCIES(lab core render)