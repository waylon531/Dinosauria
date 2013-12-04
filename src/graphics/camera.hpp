#pragma once
/** @file graphics/camera.hpp */

#include "graphics/opengl.hpp"
#include "graphics/mesh.hpp"

namespace graphics
{

  /** A camera
   * This is simply a wrapper around some matrices */
  class Camera
  {
  private:

    /** Update the matrix and direction */
    void update();
    
  public:

    glm::vec3 pos;
    glm::vec3 look;
    float aspect;
    glm::vec3 dir;

    /** The projection matrix */
    glm::mat4 mat_project;

    /** The view matrix */
    glm::mat4 mat_view;

    /** Initialize from a position and focus
     * @param pos position of camera
     * @param look point to look at */
    Camera(const glm::vec3& _pos, const glm::vec3& _look, const float _aspect=(float)W/H);

    /** Use on a mesh
     * @param material material to set */
    void use(std::shared_ptr<RenderableObjectExt> material);

    /** Set the look point
     * @param look look point */
    void setLook(const glm::vec3& _look);
      /** Set the camera position point
     * @param pos camera position point */
    void setPos(const glm::vec3& _pos);
  
  };
};
