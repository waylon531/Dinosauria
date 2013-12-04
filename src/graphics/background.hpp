#pragma once
/** @file graphics/background.hpp */

#include "graphics/buffer.hpp"
#include "graphics/glsl.hpp"
#include "graphics/rendObj.hpp"
#include "graphics/texture.hpp"

namespace graphics
{

  /** A textured background image */
  class Background: public RenderableObject
  {
  private:

    /** The background's texture */
    std::unique_ptr<GLTexture2D> texture;

    /** The shader program */
    std::unique_ptr<GLSL> shader;

    /** The vertices */
    glm::vec2* verts;

    /** The vertex buffer */
    std::unique_ptr<GLVertexBuffer> b_verts;

  public:

    /** Initialize from a texture file
     * @param fname name image image file */
    Background(const std::string& fname);

    ~Background();

    void render();
  };
};
