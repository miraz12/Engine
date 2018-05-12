#--------------------------------------------------------------------------
# core
#--------------------------------------------------------------------------

SET(files_core
	app.h
	app.cc)
SOURCE_GROUP("core" FILES ${files_core})



FILE(GLOB math_headers math/*.h)
FILE(GLOB math_sources math/*.cc)
SET(files_math 
	${math_headers}
	${math_sources})
SOURCE_GROUP("math" FILES ${files_math})

FILE(GLOB toolkit_headers toolkit/*.h)
FILE(GLOB toolkit_sources toolkit/*.cc)
SET(files_toolkit 
	${toolkit_headers}
	${toolkit_sources})
SOURCE_GROUP("toolkit" FILES ${files_toolkit})

	
SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(core STATIC ${files_core} ${files_pch} ${files_math} ${files_toolkit})
TARGET_PCH(core ../)
ADD_DEPENDENCIES(core glew)
TARGET_LINK_LIBRARIES(core PUBLIC engine exts glew)
