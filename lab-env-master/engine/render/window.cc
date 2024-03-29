//------------------------------------------------------------------------------
// window.cc
// (C) 2015-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "window.h"
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <nanovg.h>
#define NANOVG_GL3_IMPLEMENTATION 1
#include "nanovg_gl.h"



namespace Display
{


#ifdef __WIN32__
#define APICALLTYPE __stdcall
#else
#define APICALLTYPE
#endif
//------------------------------------------------------------------------------
/**
*/
static void GLAPIENTRY
GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{

	std::string msg("[OPENGL DEBUG MESSAGE] ");

	// print error severity
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_LOW:
		msg.append("\u001b[32m<Low severity> \u001b[0m");


		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		msg.append("\u001b[33m<Medium severity> \u001b[0m");
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		msg.append("\u001b[31m<High severity> \u001b[0m");
		break;
	}

	// append message to output
	msg.append(message);

	// print message
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		printf("Error: %s\n", msg.c_str());
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		printf("Performance issue: %s\n", msg.c_str());
		break;
	default:		// Portability, Deprecated, Other
		break;
	}
}

int32 Window::WindowCount = 0;
//------------------------------------------------------------------------------
/**
*/
Window::Window() :
	window(nullptr),
	vg(nullptr),
	width(1024),
	height(768),
	title("gscept Lab Environment")
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
Window::~Window()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
Window::StaticKeyPressCallback(GLFWwindow* win, int32 key, int32 scancode, int32 action, int32 mods)
{
	Window* window = (Window*)glfwGetWindowUserPointer(win);
	if (ImGui::IsMouseHoveringAnyWindow())
	{
		ImGui_ImplGlfw_KeyCallback(win, key, scancode, action, mods);
	}
    if (nullptr != window->keyPressCallback) window->keyPressCallback(key, scancode, action, mods);
}

//------------------------------------------------------------------------------
/**
*/
void
Window::StaticMousePressCallback(GLFWwindow* win, int32 button, int32 action, int32 mods)
{
	Window* window = (Window*)glfwGetWindowUserPointer(win);
	if (ImGui::IsMouseHoveringAnyWindow())
	{
		ImGui_ImplGlfw_MouseButtonCallback(win, button, action, mods);
	}
	else if (nullptr != window->mousePressCallback) window->mousePressCallback(button, action, mods);
}

//------------------------------------------------------------------------------
/**
*/
void
Window::StaticMouseMoveCallback(GLFWwindow* win, float64 x, float64 y)
{
	Window* window = (Window*)glfwGetWindowUserPointer(win);
	if (nullptr != window->mouseMoveCallback) window->mouseMoveCallback(x, y);
}

//------------------------------------------------------------------------------
/**
*/
void
Window::StaticMouseEnterLeaveCallback(GLFWwindow* win, int32 mode)
{
	Window* window = (Window*)glfwGetWindowUserPointer(win);
	if (nullptr != window->mouseLeaveEnterCallback) window->mouseLeaveEnterCallback(mode == 0);
}

//------------------------------------------------------------------------------
/**
*/
void
Window::StaticMouseScrollCallback(GLFWwindow* win, float64 x, float64 y)
{
	Window* window = (Window*)glfwGetWindowUserPointer(win);
	if (ImGui::IsMouseHoveringAnyWindow())
	{
		ImGui_ImplGlfw_ScrollCallback(win, x, y);
	}
	else if (nullptr != window->mouseScrollCallback) window->mouseScrollCallback(x, y);
}

//------------------------------------------------------------------------------
/**
*/
void
Window::StaticWindowSizeCallback(GLFWwindow* win, int width, int height)
{
    Window* window = (Window*)glfwGetWindowUserPointer(win);
    if (nullptr != window->windowResizeCallback)
    {
        window->width = width;
        window->height = height;
        window->windowResizeCallback(width, height);
        window->Resize();
    }
}
//------------------------------------------------------------------------------
/**
*/
void
Window::Resize()
{
	if (nullptr != this->window)
	{
		glfwSetWindowSize(this->window, this->width, this->height);

		// setup viewport
		glViewport(0, 0, this->width, this->height);
	}
}

//------------------------------------------------------------------------------
/**
*/
void
Window::Retitle()
{
	if (nullptr != this->window) glfwSetWindowTitle(this->window, this->title.c_str());
}

//------------------------------------------------------------------------------
/**
*/
bool
Window::Open()
{
	if (Window::WindowCount == 0)
	{
		if (!glfwInit()) return false;
	}

	// setup window
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	// open window
	this->window = glfwCreateWindow(this->width, this->height, this->title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(this->window);

	// Setup ImGui binding
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls


	if (nullptr != this->window && WindowCount == 0)
	{
		GLenum res = glewInit();
		assert(res == GLEW_OK);
		if (!(GLEW_VERSION_4_0))
		{
			printf("[WARNING]: OpenGL 4.0+ is not supported on this hardware!\n");
			glfwDestroyWindow(this->window);
			this->window = nullptr;
			return false;
		}

		// setup debug callback
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugCallback, NULL);
		GLuint unusedIds;
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);

		// setup stuff
		//glEnable(GL_FRAMEBUFFER_SRGB); //gamma correction 
		//glEnable(GL_LINE_SMOOTH);
		//glEnable(GL_POLYGON_SMOOTH);
		//glEnable(GL_MULTISAMPLE);
		//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		// setup viewport
		glViewport(0, 0, this->width, this->height);
	}


	glfwSetWindowUserPointer(this->window, this);
	glfwSetKeyCallback(this->window, Window::StaticKeyPressCallback);
	glfwSetMouseButtonCallback(this->window, Window::StaticMousePressCallback);
	glfwSetCursorPosCallback(this->window, Window::StaticMouseMoveCallback);
	glfwSetCursorEnterCallback(this->window, Window::StaticMouseEnterLeaveCallback);
	glfwSetScrollCallback(this->window, Window::StaticMouseScrollCallback);
    glfwSetWindowSizeCallback(this->window, Window::StaticWindowSizeCallback);
	// setup imgui implementation
	ImGui_ImplGlfwGL3_Init(this->window, false);
	glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);

	// setup nanovg
	this->vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

	// increase window count and return result
	Window::WindowCount++;
	return this->window != nullptr;
}

//------------------------------------------------------------------------------
/**
*/
void
Window::Close()
{
	if (nullptr != this->window) glfwDestroyWindow(this->window);
	this->window = nullptr;
	Window::WindowCount--;
	if (Window::WindowCount == 0)
	{
		ImGui_ImplGlfwGL3_Shutdown();
		glfwTerminate();
        exit(0);
	}
}

//------------------------------------------------------------------------------
/**
*/
void
Window::MakeCurrent()
{
	glfwMakeContextCurrent(this->window);
	ImGui::DestroyContext();

}

//------------------------------------------------------------------------------
/**
*/
void
Window::Update()
{
	glfwPollEvents();	
}

//------------------------------------------------------------------------------
/**
*/
void
Window::SwapBuffers()
{
	if (this->window)
	{
		if (nullptr != this->nanoFunc)
		{
			int32 fbWidth, fbHeight;
			glClear(GL_STENCIL_BUFFER_BIT);
			glfwGetWindowSize(this->window, &this->width, &this->height);
			glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
			nvgBeginFrame(this->vg, this->width, this->height, (float)fbWidth / (float) this->width);
			this->nanoFunc(this->vg);
			nvgEndFrame(this->vg);
		}
		if (nullptr != this->uiFunc)
		{
			ImGui_ImplGlfwGL3_NewFrame();
			this->uiFunc();
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		}
		glfwSwapBuffers(this->window);
	}
}

} // namespace Display