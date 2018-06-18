#--------------------------------------------------------------------------
# render
#--------------------------------------------------------------------------

SET(files_render_display
	window.h
	window.cc
	camera.h
	camera.cc)
SOURCE_GROUP("display" FILES ${files_render_display})

FILE(GLOB resources_headers resources/*.h)
FILE(GLOB resources_sources resources/*.cc)
SET(files_resources 
	${resources_headers}
	${resources_sources})
SOURCE_GROUP("resources" FILES ${files_resources})

FILE(GLOB properties_headers properties/*.h)
FILE(GLOB properties_sources properties/*.cc)
SET(files_properties 
	${properties_headers}
	${properties_sources})
SOURCE_GROUP("properties" FILES ${files_properties})

FILE(GLOB particles_headers particles/*.h)
FILE(GLOB particles_sources particles/*.cc)
SET(files_particles 
	${particles_headers}
	${particles_sources})
SOURCE_GROUP("particles" FILES ${files_particles})

FILE(GLOB input_headers input/*.h)
FILE(GLOB input_sources input/*.cc)
SET(files_input 
	${input_headers}
	${input_sources})
SOURCE_GROUP("input" FILES ${files_input})

FILE(GLOB managers_headers managers/*.h)
FILE(GLOB managers_sources managers/*.cc)
SET(files_managers 
	${managers_headers}
	${managers_sources})
SOURCE_GROUP("managers" FILES ${files_managers})

FILE(GLOB base_headers base/*.h)
FILE(GLOB base_sources base/*.cc)
SET(files_base 
	${base_headers}
	${base_sources})
SOURCE_GROUP("base" FILES ${files_base})

FILE(GLOB skybox_headers skybox/*.h)
FILE(GLOB skybox_sources skybox/*.cc)
SET(files_skybox 
	${skybox_headers}
	${skybox_sources})
SOURCE_GROUP("skybox" FILES ${files_skybox})

SET(files_render
	${files_render_display}
	${files_render_pch}
	${files_resources}
	${files_properties}
	${files_particles}
	${files_input}
	${files_managers}
	${files_base}
	${files_skybox})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(render STATIC ${files_render} ${files_pch})
TARGET_PCH(render ../)	
ADD_DEPENDENCIES(render core assimp glew glfw)
TARGET_LINK_LIBRARIES(render PUBLIC engine exts glew glfw imgui ${OPENGL_LIBS})