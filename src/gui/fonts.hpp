#pragma once
/** @file gui/fonts.hpp These are global fonts that will be loaded */

#include "graphics/text.hpp"

namespace gui
{

  extern std::shared_ptr<graphics::DynamicFont> font_50pt;
  extern std::shared_ptr<graphics::DynamicFont> font_100pt;
  extern std::shared_ptr<graphics::DynamicFont> font_150pt;
  extern std::shared_ptr<graphics::DynamicFont> font_200pt;

  extern void initFonts();
};
