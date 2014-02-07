#include "graphics/base.hpp"
using namespace graphics;

#include <cassert>
#include <cstdlib>

//global data

char keyOnce[GLFW_KEY_LAST + 1];
char mouseOnce[GLFW_MOUSE_BUTTON_LAST + 1];

//this is the GL context lock
bool glLock = false;

graphics::GLViewport* graphics::current_viewport = NULL;

//GLContext methods

void APIENTRY DebugOutputCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam){

	printf("OpenGL Debug Output message : ");

	if(source == GL_DEBUG_SOURCE_API_ARB)					printf("Source : API; ");
	else if(source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)	printf("Source : WINDOW_SYSTEM; ");
	else if(source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)	printf("Source : SHADER_COMPILER; ");
	else if(source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB)		printf("Source : THIRD_PARTY; ");
	else if(source == GL_DEBUG_SOURCE_APPLICATION_ARB)		printf("Source : APPLICATION; ");
	else if(source == GL_DEBUG_SOURCE_OTHER_ARB)			printf("Source : OTHER; ");

	if(type == GL_DEBUG_TYPE_ERROR_ARB)						printf("Type : ERROR; ");
	else if(type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB)	printf("Type : DEPRECATED_BEHAVIOR; ");
	else if(type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)	printf("Type : UNDEFINED_BEHAVIOR; ");
	else if(type == GL_DEBUG_TYPE_PORTABILITY_ARB)			printf("Type : PORTABILITY; ");
	else if(type == GL_DEBUG_TYPE_PERFORMANCE_ARB)			printf("Type : PERFORMANCE; ");
	else if(type == GL_DEBUG_TYPE_OTHER_ARB)				printf("Type : OTHER; ");

	if(severity == GL_DEBUG_SEVERITY_HIGH_ARB)				printf("Severity : HIGH; ");
	else if(severity == GL_DEBUG_SEVERITY_MEDIUM_ARB)		printf("Severity : MEDIUM; ");
	else if(severity == GL_DEBUG_SEVERITY_LOW_ARB)			printf("Severity : LOW; ");

	// You can set a breakpoint here ! Your debugger will stop the program,
	// and the callstack will immediately show you the offending call.
	printf("Message : %s\n", message);
}

GLContext::GLContext(const int Width, const int Height, const char* title)
{
  //assert(glLock==false && "There can only be one OpenGL window open at a time");
  width = Width;
  height = Height;
  glLock = true;
  //initialize glfw
  assert(glfwInit() && "GLFW initialization failed");
  //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_FALSE);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,GL_TRUE);
  //glfwWindowHint(GLFW_SAMPLES,4);
  window = glfwCreateWindow(width,height,title,NULL,NULL);
  assert(window && "GLFW window creation failed");
	
  this->use();
  //glewExperimental=true;
  assert(glewInit()==0 && "GLEW initialization failed");
	
  //get opengl info
#ifdef DEBUG
  std::cout << "OpenGL info: " << std::endl
	    << "  Version: " << glGetString(GL_VERSION) << std::endl
	    << "  GLSL_Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl
	    << "  Vendor: " << glGetString(GL_VENDOR) << std::endl;
  //<< "  Extensions: " << glGetString(GL_EXTENSIONS) << std::endl;
#endif
	
  assert(glewIsSupported("GL_ARB_vertex_shader"));
  //assert(glewIsSupported("GL_geometry_shader"));
  assert(glewIsSupported("GL_ARB_fragment_shader"));

  glDebugMessageCallbackARB(&DebugOutputCallback, NULL);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB); 
  
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
}

GLContext::~GLContext()
{
  /*
  assert(glLock=true && "Either there is a serious bug, or the glLock has bee tampered with (DO NOT EDIT glLock)");
  //stop glfw
  glfwDestroyWindow(window);
  glfwTerminate();
  //set lock
  glLock = false;*/
}

void GLContext::use()
{
  glfwMakeContextCurrent(window);
}

const bool GLContext::getEventClose()
{
  return glfwWindowShouldClose(window);
}

void GLContext::pollEvents()
{
  glfwPollEvents();
}

void GLContext::flip()
{
  glfwSwapBuffers(window);
  usleep(10000);
}

GLFWwindow* GLContext::getWindow() const
{
  return window;
}

const int GLContext::getWidth() const
{
  return width;
}

const int GLContext::getHeight() const
{
  return height;
}

//GLViewport methods
#include "graphics/glsl.hpp"

GLViewport::GLViewport(const int X, const int Y, const int Width, const int Height)
{
  assert(glLock==true && "There is no window, and therefore no OpenGL context to create a viewport for!");
  width = Width;
  height = Height;
  x = X;
  y = Y;
	
  objects = std::vector< std::weak_ptr<RenderableObject> >();
  //  mat_ortho = glm::ortho(x,width,y,height);
  mat_ortho = glm::ortho(0.f,(float)width,1.f,(float)height);
  unif_ortho = std::shared_ptr<GLSLUniform>(new GLSLUniform("m_ortho",&mat_ortho));
}

GLViewport::~GLViewport()
{
  //objects are allocated on the stack and do not need to bee dealloced
  unif_ortho.reset();
}

void GLViewport::addObject(std::weak_ptr<RenderableObject> target)
{
  objects.push_back(target);
}

void GLViewport::use() const
{
  glEnable(GL_SCISSOR_TEST);
  glScissor(x,y,width,height);
  glViewport(x,y,width,height);
  current_viewport = const_cast<GLViewport*>(this);
}

void GLViewport::render()
{
  use();
  for(std::vector<std::weak_ptr<RenderableObject>>::iterator target 
	= objects.begin();
      target != objects.end(); ++target)
    {
      std::shared_ptr<RenderableObject>(*target)->render();
    }
}

const int GLViewport::getX() const
{
  return x;
}

const int GLViewport::getY() const
{
  return y;
}

const int GLViewport::getWidth() const
{
  return width;
}

const int GLViewport::getHeight() const
{
  return height;
}
