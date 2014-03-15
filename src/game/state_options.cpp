#include "game/state_options.hpp"

//external includes
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIPushButton.h>
#include <elements/CEGUIRadioButton.h>
//custom includes
#include "engine/state_manager.hpp"
#include "engine/settings.hpp"
#include "graphics/context.hpp"

OptionsState::OptionsState()
{
  guiRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout("options.layout");

  ((CEGUI::PushButton*)CEGUI::WindowManager::getSingleton().getWindow("Options/Close"))->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OptionsState::Close_onClick, this));

  ((CEGUI::RadioButton*)CEGUI::WindowManager::getSingleton().getWindow("Options/GraphicsLow"))->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OptionsState::Graphics_onStateChange, this));

  ((CEGUI::RadioButton*)CEGUI::WindowManager::getSingleton().getWindow("Options/GraphicsMedium"))->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OptionsState::Graphics_onStateChange, this));
  
  ((CEGUI::RadioButton*)CEGUI::WindowManager::getSingleton().getWindow("Options/GraphicsHigh"))->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OptionsState::Graphics_onStateChange, this));

  ((CEGUI::RadioButton*)CEGUI::WindowManager::getSingleton().getWindow("Options/GraphicsExtreme"))->subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OptionsState::Graphics_onStateChange, this));
  
  if(engine::userSettings["graphicsMode"] == "low")
    {
      ((CEGUI::RadioButton*)(CEGUI::WindowManager::getSingleton().getWindow("Options/GraphicsLow")))->setSelected(true);
    }
  else if(engine::userSettings["graphicsMode"] == "medium")
    {
      ((CEGUI::RadioButton*)(CEGUI::WindowManager::getSingleton().getWindow("Options/GraphicsMedium")))->setSelected(true);
    }
  else if(engine::userSettings["graphicsMode"] == "high")
    {
      ((CEGUI::RadioButton*)(CEGUI::WindowManager::getSingleton().getWindow("Options/GraphicsHigh")))->setSelected(true);
    }
  else if(engine::userSettings["graphicsMode"] == "extreme")
    {
      ((CEGUI::RadioButton*)(CEGUI::WindowManager::getSingleton().getWindow("Options/GraphicsExtreme")))->setSelected(true);
    }
}

OptionsState::~OptionsState()
{
  delete guiRoot;
}

void OptionsState::enter()
{
  CEGUI::System::getSingleton().setGUISheet(guiRoot);
  graphics::OgreEngine::getSingleton().setupGUIMgr();
}

void OptionsState::exit()
{
}

void OptionsState::pause()
{
}

void OptionsState::resume()
{
  enter();
}

bool OptionsState::Close_onClick(const CEGUI::EventArgs& args)
{
  engine::StateManager::getSingleton().popState();
  return true;
}

bool OptionsState::Graphics_onStateChange(const CEGUI::EventArgs& args)
{
  unsigned int id = ((CEGUI::RadioButton*)(((CEGUI::WindowEventArgs&)args).window))->getSelectedButtonInGroup()->getID();
  switch(id)
    {
    case 0:
      engine::userSettings["graphicsMode"] = "low";
      break;
    case 1:
      engine::userSettings["graphicsMode"] = "medium";
      break;
    case 2:
      engine::userSettings["graphicsMode"] = "high";
      break;
    case 3:
      engine::userSettings["graphicsMode"] = "extreme";
      break;
    default:
      std::cerr << "Invalid button id: " << id << std::endl;
    }
  engine::saveSettings();
  
  engine::StateManager::getSingleton().onGraphicsSettings();
  return true;
}
