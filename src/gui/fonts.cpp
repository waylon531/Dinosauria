#include "gui/fonts.hpp"

std::shared_ptr<graphics::DynamicFont> gui::font_50pt;
std::shared_ptr<graphics::DynamicFont> gui::font_100pt;
std::shared_ptr<graphics::DynamicFont> gui::font_150pt;
std::shared_ptr<graphics::DynamicFont> gui::font_200pt;

void gui::initFonts()
{
  font_50pt = std::shared_ptr<graphics::DynamicFont>(new graphics::DynamicFont("res/fonts/a_charming_font/Achafont.ttf",50));
  font_100pt = std::shared_ptr<graphics::DynamicFont>(new graphics::DynamicFont("res/fonts/a_charming_font/Achafont.ttf",100));
  font_150pt = std::shared_ptr<graphics::DynamicFont>(new graphics::DynamicFont("res/fonts/a_charming_font/Achafont.ttf",150));
  font_200pt = std::shared_ptr<graphics::DynamicFont>(new graphics::DynamicFont("res/fonts/a_charming_font/Achafont.ttf",200));
}
