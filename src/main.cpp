//file: main.cpp
//purpose: main program execution

//external includes
//custom includes
#include "util.hpp"
#include "graphics/context.hpp"
#include "engine/state.hpp"
#include "engine/settings.hpp"
#include "engine/state_manager.hpp"
#include "game/state_mmenu.hpp"
#include "game/state_options.hpp"
#include "game/state_test.hpp" //REMOVE THIS

int main(int argc, char** argv)
{
  engine::loadSettings();
  
  graphics::OgreEngine::getSingleton().startup();

  engine::StateManager::getSingleton().addState(new MainMenuState);
  engine::StateManager::getSingleton().addState(new OptionsState);
  engine::StateManager::getSingleton().addState(new PrototypeState);
  engine::StateManager::getSingleton().switchState("main_menu");

  engine::StateManager::getSingleton().onGraphicsSettings();
  
  engine::StateManager::getSingleton().isRunning = true;
  graphics::OgreEngine::getSingleton().startRenderLoop();
  //engine::StateManager::getSingleton().start();
  
  graphics::OgreEngine::getSingleton().shutdown();
  return 0;
}
