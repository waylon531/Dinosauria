#pragma once

//external includes
#include <CEGUIEvent.h>
//custom includes
#include "engine/state.hpp"

namespace CEGUI
{
  class Window;
};

/** A state for the options configuration window */
class OptionsState: public engine::GameState
{
private:

  /** The GUI root window */
  CEGUI::Window* guiRoot;
  
public:

  std::string getName() const
  {
    return "options";
  }

  /** Callback for window close button */
  bool Close_onClick(const CEGUI::EventArgs& args);

  /** Callback for graphics quality settings */
  bool Graphics_onStateChange(const CEGUI::EventArgs& args);
  
  /** Initialize */
  OptionsState();

  /** Destroy */
  ~OptionsState();

  void enter();
  void exit();
  void pause();
  void resume();
};
