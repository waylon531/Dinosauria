#pragma once
/** @file graphics/rendObj.hpp Renderable object handling */

#include "graphics/opengl.hpp"

namespace graphics
{
	
  /** @brief An abstract object that can render in OpenGL
   * 
   * These are used by viewports as wrappers around arbitrary sets of OpenGL
   * function calls */
  class RenderableObject
  {
  public:
    //public methods
		
    /** @brief Preform functions calls for OpenGL
     * 
     * This is really the only part of this class. */
    virtual void render() =0;
  };

  /** An abstrace object with extra methods to set transformation matrices */
  class RenderableObjectExt: public RenderableObject
  {
  public:

    /** Render a shadow pass */
    virtual void renderShadowPass() {render();}

    /** Set the view matrix */
    virtual void setMatrixView(glm::mat4* view) =0;

    /** Set the projection matrix */
    virtual void setMatrixProject(glm::mat4* project) =0;

    /** Set the eye direction */
    virtual void setEyeDir(glm::vec3* dir) =0;
    /** Set the sun direction */
    virtual void setSunDir(glm::vec3* dir) =0;

    /** Set the shadow texture id */
    virtual void setShadowTex(GLint* v) =0;
    /** Set the light transform matrix */
    virtual void setMatrixLight(glm::mat4* m) =0;
  };

}
	
