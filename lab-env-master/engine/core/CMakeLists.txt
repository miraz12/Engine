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

FILE(GLOB weather_headers weatherinfo/*.h)
FILE(GLOB weather_sources weatherinfo/*.cc)
SET(files_weather 
	${weather_headers}
	${weather_sources})
SOURCE_GROUP("weatherinfo" FILES ${files_weather})

	
SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(core STATIC ${files_core} ${files_pch} ${files_math} ${files_weather})
TARGET_PCH(core ../)
ADD_DEPENDENCIES(core glew)
TARGET_LINK_LIBRARIES(core PUBLIC engine exts glew)
