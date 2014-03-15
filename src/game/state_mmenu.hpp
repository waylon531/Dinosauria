#pragma once

//external includes
#include <CEGUIEvent.h>
//custom includes
#include "engine/state.hpp"

namespace CEGUI
{
  class Window;
};

/** A state for the main game menu */
class MainMenuState: public engine::GameState
{
private:

  /** The GUI root window */
  CEGUI::Window* guiRoot;

public:

  std::string getName() const
  {
    return "main_menu";
  }
  
  /** Callback for quit button */
  bool Quit_onClick(const CEGUI::EventArgs& args);

  /** Callback for options button */
  bool Options_onClick(const CEGUI::EventArgs& args);

  /** Callback for start game button */
  bool StartGame_onClick(const CEGUI::EventArgs& args);

  /** Initialize */
  MainMenuState();

  /** Destroy */
  ~MainMenuState();

  void enter();
  void exit();
  void pause();
  void resume();
};
