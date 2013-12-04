#pragma once
/** @file gui/pbar.hpp */

#include "gui/widget.hpp"
#include "graphics/buffer.hpp"
#include "graphics/glsl.hpp"

namespace gui
{

  /** A progress bar */
  class ProgressBar: public Widget
  {
  private:

    glm::vec2* verts_back;
    std::shared_ptr<graphics::GLVertexBuffer> b_verts_back;
    glm::vec2* verts_front;
    std::shared_ptr<graphics::GLVertexBufferDynamic> b_verts_front;
    std::shared_ptr<graphics::GLSL> shader;
    glm::vec4 color;
    float length;
    glm::vec2 position;
    std::shared_ptr<graphics::GLSLUniform> u_color;
    
  public:

    /** A number from 0 to 1 for the progress */
    float value;

    /** Initialize from a position and width
     * @param pos position of bar
     * @param len length of bar */
    ProgressBar(const glm::vec2& pos, const float len);

    ~ProgressBar();

    /** Update the dynamic vertex buffer */
    void update();
    
    void render();
  };
};
    
