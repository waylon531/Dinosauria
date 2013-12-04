#pragma once
/** @file gui/button.hpp */

#include "gui/widget.hpp"
#include "graphics/buffer.hpp"
#include "graphics/ibo.hpp"
#include "gui/text.hpp"

namespace gui
{
  /** A basic clickable button widget */
  class Button: public Widget
  {
  private:

    /** The text object */
    std::unique_ptr<StaticText> text;

    /** The callback function when clicked */
    Callback callback;

    /** The vertex list */
    glm::vec2* verts;
    /** The index list */
    GLuint* inds;

    /** The vertex buffer */
    std::unique_ptr<graphics::GLVertexBuffer> b_verts;
    /** The index buffer */
    std::unique_ptr<graphics::GLIndexBuffer> b_inds;

    /** The shader */
    std::unique_ptr<graphics::GLSL> shader;

    /** The color of the button */
    glm::vec4 color;
    /** The color uniform */
    std::shared_ptr<graphics::GLSLUniform> u_color;

  public:

    /** Initialize from text and a callback
     * @param txt text string for button
     * @param pos button position
     * @param clb callback function */
    Button(const std::string& txt, const glm::vec2& pos, Callback clb, std::shared_ptr<graphics::GLSLUniform> uniform=graphics::u_ortho);

    ~Button();

    void render();
    void onMouseOver(const glm::vec2& pos);
    void onNotMouseOver();
    void onMouseClick(const glm::vec2& pos);
    void onMouseRelease(const glm::vec2& pos);
  };
};
