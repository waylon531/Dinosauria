#include "gui/text.hpp"

gui::StaticText::StaticText(const std::string& txt, const glm::vec4& color, const glm::vec2& pos, std::shared_ptr<graphics::DynamicFont> font, std::shared_ptr<graphics::GLSLUniform> uniform) : Widget(pos)
{
  text = std::unique_ptr<graphics::StaticText>(new graphics::StaticText(font, txt, position, color, uniform));
  bbox = text->bbox;
}

gui::StaticText::~StaticText()
{
  text.reset();
}

void gui::StaticText::render()
{
  text->render();
}
