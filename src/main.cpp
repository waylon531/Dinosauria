//file: main.cpp
//purpose: main program execution

//custom external includes
#include "luabind/luabind.hpp"
//custom includes
#include "util.hpp"
#include "graphics/context.hpp"
#include "engine/state.hpp"
#include "engine/settings.hpp"
#include "engine/state_manager.hpp"
#include "scripting/context.hpp"
#include "scripting/bindings.hpp"
#include "game/state_mmenu.hpp"
#include "game/state_options.hpp"
#include "game/state_test.hpp" //REMOVE THIS

int main(int argc, char** argv)
{
  try
    {
      engine::loadSettings();
      
      graphics::OgreEngine::getSingleton().startup();
      engine::LuaEngine::getSingleton().startup();
      
      initLuaBindings();
      
      engine::LuaEngine::getSingleton().loadAllScripts();
      
      //engine::StateManager::getSingleton().addState(new MainMenuState);
      //engine::StateManager::getSingleton().addState(new OptionsState);
      //engine::StateManager::getSingleton().addState(new PrototypeState);
      engine::StateManager::getSingleton().switchState("main_menu");
      
      engine::StateManager::getSingleton().onGraphicsSettings();
      
      engine::StateManager::getSingleton().isRunning = true;
      graphics::OgreEngine::getSingleton().startRenderLoop();
      //engine::StateManager::getSingleton().start();
      
    }
  catch(luabind::error e)
    {
      luabind::object error_msg(luabind::from_stack(e.state(), -1));
      std::cout << "Lua runtime error: " << error_msg << std::endl;
    }
  
  graphics::OgreEngine::getSingleton().shutdown();
  engine::LuaEngine::getSingleton().shutdown();
  return 0;
}
