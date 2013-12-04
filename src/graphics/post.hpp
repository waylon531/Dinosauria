#pragma once
/** @file graphics/post.hpp */

#include "graphics/fbo.hpp"

namespace graphics
{

  /** A Post compoisitor (complete) */
  class Compositor
  {
  private:

    /** The vertex buffer data */
    glm::vec2* verts;
    
    /** The vertex buffer used to draw to texture */
    std::unique_ptr<GLVertexBuffer> b_vert;

  public:

    /** The postprocessing shader */
    std::shared_ptr<GLSL> shader;

    /** Initialize
     * @param glsl shader to use */
    Compositor(std::shared_ptr<graphics::GLSL> glsl);

    /** Destroy */
    ~Compositor();

    /** Draw the composite */
    void draw();
  };

};
