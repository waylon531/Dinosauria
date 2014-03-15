#include "game/state_mmenu.hpp"

//external includes
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIPushButton.h>
//custom includes
#include "engine/state_manager.hpp"
#include "graphics/context.hpp"

MainMenuState::MainMenuState()
{
  guiRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout("main_menu.layout");
  
  ((CEGUI::PushButton*)CEGUI::WindowManager::getSingleton().getWindow("MainMenu/Quit"))->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuState::Quit_onClick, this));

  ((CEGUI::PushButton*)CEGUI::WindowManager::getSingleton().getWindow("MainMenu/Options"))->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuState::Options_onClick, this));

  ((CEGUI::PushButton*)CEGUI::WindowManager::getSingleton().getWindow("MainMenu/StartGame"))->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuState::StartGame_onClick, this));
}

MainMenuState::~MainMenuState()
{
  delete guiRoot;
}

void MainMenuState::enter()
{
  CEGUI::System::getSingleton().setGUISheet(guiRoot);
  graphics::OgreEngine::getSingleton().setupGUIMgr();
}

void MainMenuState::exit()
{
}

void MainMenuState::pause()
{
}

void MainMenuState::resume()
{
  enter();
}

bool MainMenuState::Quit_onClick(const CEGUI::EventArgs& args)
{
  engine::StateManager::getSingleton().isRunning = false;
  return true;
}

bool MainMenuState::Options_onClick(const CEGUI::EventArgs& args)
{
  engine::StateManager::getSingleton().pushState("options");
  return true;
}

bool MainMenuState::StartGame_onClick(const CEGUI::EventArgs& args)
{
  engine::StateManager::getSingleton().switchState("test");
  return true;
}
