
#pragma once
/** @file graphics/base.cpp Base OpenGL renderer */

#include "graphics/opengl.hpp"
#include "graphics/rendObj.hpp"

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <csignal>

namespace graphics
{
	
  /** @brief Class for GLFW window/opengl context
   * 
   * There should only be one of these at a time so it will set a lock
   * and call an error if the lock has already been set */
  class GLContext
  {
  private:
    //private data
    /** @brief Window for this context */
    GLFWwindow* window;
		
    /** @brief Width of window */
    int width;
		
    /** @brief Height of wihdow */
    int height;
  public:
		
    /** @brief Open a window
     *
     * This also locks the GL_LOCK.
     * @param Width width of window
     * @param Height height of window 
     * @param title title of window */
    GLContext(const int Width, const int Height, const char* title);
		
    /** @brief Stop GLFW, close window, and reset lock */
    ~GLContext();
		
    /** @brief Set as current context */
    void use();
		
    /** @brief Check if the window should close 
     * 
     * @return whether the window has been closed */
    const bool getEventClose();
		
    /** @brief Poll events of window */
    void pollEvents();
		
    /** @brief Update window
     * 
     * This will swap the double buffers */
    void flip();
		
    /** @brief Get pointer to the window
     * 
     * @return pointer to window controlled by this */
    GLFWwindow* getWindow() const;
		
    /** @brief Get width of window
     * 
     * @return width of window */
    const int getWidth() const;
		
    /** @brief Get height of window
     * 
     * @return height of window */
    const int getHeight() const;
		
  };

  class GLSLUniform;
  /** @brief Class for OpenGL viewport
   * 
   * All viewports have seperate settings */
  class GLViewport
  {
  private:
    //private data
		
    /** @brief X position of viewport */
    int x;
		
    /** @brief Y position of viewport */
    int y;
		
    /** @brief Width of viewport */
    int width;
		
    /** @brief Height of viewport */
    int height;
		
    /** @brief List of RenderableObject that belong to this viewport */
    std::vector< std::weak_ptr<graphics::RenderableObject> > objects;
		
  public:
    
    /** The orthographics projection matrix for this viewport */
    glm::mat4 mat_ortho;
    /** The uniform for the orthographics projection matrix */
    std::shared_ptr<GLSLUniform> unif_ortho;
	  
    //public methods
		
    /** @brief Create viewport
     * 
     * @param X x position of viewport
     * @param Y y position of viewport
     * @param Width width of viewport
     * @param Height height of viewport */
    GLViewport(const int X, const int Y, const int Width, const int Height);
		
    /** @brief Destroy viewport */
    ~GLViewport();
		
    /** @brief Add a renderable object to this viewport as a render target
     * 
     * Note that these must be added in the order that they are intended to
     * be rendered in. If there is a RenderableObject to setup the matrices,
     * then that should be added first.
     * @param target renderable object */
    void addObject(std::weak_ptr<graphics::RenderableObject> target);
		
    /** @brief Setup OpenGL to use this viewport
     * 
     * Only one viewport can be active at a time */
    void use() const;
		
    /** @brief Render all components of this viewport 
     * 
     * This will call the use method so there is no need to do that beforehand. */
    void render();
		
    /** @brief Get x
     * 
     * @return x position of viewport */
    const int getX() const;
		
    /** @brief Get y
     * 
     * @return y position of viewport */
    const int getY() const;
		
    /** @brief Get width
     * 
     * @return width of viewport */
    const int getWidth() const;
		
    /** @brief Get height
     * 
     * @return height of viewport */
    const int getHeight() const;
  };
  /** The currently active viewport */
  extern GLViewport* current_viewport;
}
