#pragma once
/** @file gui/text.hpp Static and dynamic text widgets */

#include "gui/widget.hpp"
#include "gui/fonts.hpp"
#include "graphics/text.hpp"

namespace gui
{

  /** Static text widget */
  class StaticText: public Widget
  {
  protected:

    /** The wrapped text */
    std::unique_ptr<graphics::StaticText> text;

  public:

    /** Initialize from text, color, position, and font
     * @param txt text string
     * @param color text color
     * @param pos position
     * @param font text font */
    StaticText(const std::string& txt, const glm::vec4& color, const glm::vec2& pos, std::shared_ptr<graphics::DynamicFont> font = font_50pt, std::shared_ptr<graphics::GLSLUniform> uniform=graphics::u_ortho);

    ~StaticText();

    void render();
  };
};
