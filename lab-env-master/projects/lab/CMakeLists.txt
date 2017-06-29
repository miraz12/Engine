#--------------------------------------------------------------------------
# lab project
#--------------------------------------------------------------------------

PROJECT(lab)
FILE(GLOB lab_headers code/*.h)
FILE(GLOB lab_sources code/*.cc)

SET(files_lab ${lab_headers} ${lab_sources} code/plane.cc code/plane.h code/ray.cc code/ray.h code/AABB.cc code/AABB.h code/physicsnode.cc code/physicsnode.h code/collisionhandler.cc code/collisionhandler.h code/simplex.cc code/simplex.h code/particlesystem.cc code/particlesystem.h code/cell.cc code/cell.h code/fluidsimulation.cc code/fluidsimulation.h code/pressuresolver.cc code/pressuresolver.h code/mactable.cc code/mactable.h code/array3d.h)
SOURCE_GROUP("lab" FILES ${files_lab})

ADD_EXECUTABLE(lab ${files_lab})
TARGET_LINK_LIBRARIES(lab core render)
ADD_DEPENDENCIES(lab core render)