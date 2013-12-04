#pragma once
/** @file graphics/opengl.hpp OpenGL included header files */

#include <GL/glew.h>

#include <GLFW/glfw3.h>
extern char keyOnce[GLFW_KEY_LAST + 1];
#define glfwGetKeyOnce(WINDOW, KEY)			\
  (glfwGetKey(WINDOW,KEY) ?				\
  (keyOnce[KEY] ? false : (keyOnce[KEY] = true)) :	\
   (keyOnce[KEY] = false))
extern char mouseOnce[GLFW_MOUSE_BUTTON_LAST + 1];
#define glfwGetMouseButtonOnce(WINDOW, KEY)			\
  (glfwGetMouseButton(WINDOW,KEY) ?				\
   (mouseOnce[KEY] ? false : (mouseOnce[KEY] = true)) :		\
   (mouseOnce[KEY] = false))

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

//#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#define W 1024
//#define H 1024
#define REAL_W 1280
#define REAL_H 1024
#define W REAL_W
#define H REAL_H
