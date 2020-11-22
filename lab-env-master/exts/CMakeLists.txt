#--------------------------------------------------------------------------
# exts
#--------------------------------------------------------------------------

ADD_LIBRARY(exts INTERFACE)

ADD_SUBDIRECTORY(glew)
SET_TARGET_PROPERTIES(glew PROPERTIES FOLDER "exts/glew")
TARGET_INCLUDE_DIRECTORIES(exts INTERFACE glew/include)

SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
SET(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
SET(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")		  

ADD_SUBDIRECTORY(glfw-3.1.1)
SET_TARGET_PROPERTIES(glfw PROPERTIES FOLDER "exts/glfw")
TARGET_INCLUDE_DIRECTORIES(exts INTERFACE glfw-3.1.1/include)

ADD_SUBDIRECTORY(yaml-cpp)
SET_TARGET_PROPERTIES(yaml-cpp PROPERTIES FOLDER "exts/yaml-cpp")
TARGET_INCLUDE_DIRECTORIES(exts INTERFACE yaml-cpp/include)

ADD_SUBDIRECTORY(assimp-4.1.0)
SET_TARGET_PROPERTIES(assimp PROPERTIES FOLDER "exts/assimp")
TARGET_INCLUDE_DIRECTORIES(exts INTERFACE assimp-4.1.0/include)
TARGET_INCLUDE_DIRECTORIES(exts INTERFACE ${CMAKE_BINARY_DIR}/exts/assimp-4.1.0/include)

ADD_LIBRARY(nanovg STATIC nanovg/src/nanovg.c nanovg/src/nanovg.h nanovg/src/nanovg_gl.h nanovg/src/nanovg_gl_utils.h)
TARGET_LINK_LIBRARIES(nanovg PUBLIC exts)
TARGET_INCLUDE_DIRECTORIES(nanovg PUBLIC nanovg/src)
SET_TARGET_PROPERTIES(nanovg PROPERTIES FOLDER "exts/nanovg")

ADD_LIBRARY(imgui STATIC imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp 
			imgui/imgui.h imgui/imgui_internal.h 
			imgui/stb_rect_pack.h imgui/stb_textedit.h imgui/stb_truetype.h 
			imgui/examples/opengl3_example/imgui_impl_glfw_gl3.h
			imgui/examples/opengl3_example/imgui_impl_glfw_gl3.cpp)
			


TARGET_LINK_LIBRARIES(imgui PUBLIC exts)
TARGET_INCLUDE_DIRECTORIES(imgui PUBLIC imgui imgui/examples/opengl3_example)
SET_TARGET_PROPERTIES(imgui PROPERTIES FOLDER "exts/imgui")

TARGET_LINK_LIBRARIES(exts INTERFACE imgui nanovg assimp yaml-cpp)
