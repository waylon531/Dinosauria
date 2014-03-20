//external includes
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIWindow.h>
//custom external includes
#include "luabind/luabind.hpp"
#include "luabind/adopt_policy.hpp"
#include "luabind/operator.hpp"
//custom includes
#include "graphics/context.hpp"
#include "scripting/context.hpp"
#include "engine/state.hpp"
#include "engine/state_manager.hpp"
#include "engine/settings.hpp"
#include "engine/component.hpp"
#include "engine/entity.hpp"
#include "engine/entity_mgr.hpp"
#include "engine/world.hpp"
#include "engine/system.hpp"

struct wrap_GameState : engine::GameState, luabind::wrap_base
{
  wrap_GameState()
  {
  }

  virtual std::string getName() const
  {
    return call<std::string>("getName");
  }

  virtual void enter()
  {
    call<void>("enter");
  }

  virtual void exit()
  {
    call<void>("exit");
  }

  virtual void pause()
  {
    call<void>("pause");
  }

  virtual void resume()
  {
    call<void>("resume");
  }

  virtual void onTick(const float pulse)
  {
    call<void>("onTick",pulse);
  }

  static void default_onTick(GameState* self, const float pulse)
  {
  }
};

struct wrap_Component : engine::Component, luabind::wrap_base
{
  wrap_Component()
  {
  }
  
  virtual std::string getName() const
  {
    return call<std::string>("getName");
  }
};

struct wrap_System : engine::System, luabind::wrap_base
{
  wrap_System()
  {
  }

  virtual void run(const float pulse)
  {
    call<void>("run",pulse);
  }
};

/*
class LuaObjectComponent: public engine::Component
{
public:

  virtual std::string getName() const
  {
    return "YOU_SHOULD_NEVER_SEE_THIS(LuaObjectComponent raw)";
  }
  
  luabind::object object;

  LuaObjectComponent(luabind::object obj) : object(obj)
  {
  }

  LuaObjectComponent()
  {
  }
};

static LuaObjectComponent* Entity_getComponent_int(engine::Entity* self, const unsigned int id)
{
  return static_cast<LuaObjectComponent*>(self->getComponent(id).get());
}

static LuaObjectComponent* Entity_getComponent_str(engine::Entity* self, const std::string& name)
{
  return static_cast<LuaObjectComponent*>(self->getComponent(name).get());
}
*/
/*CEGUI::Window* loadGUILayout(const std::string& fname)
{
  return CEGUI::WindowManager::getSingleton().loadWindowLayout(fname);
}

void setGUILayout(CEGUI::Window* guiRoot)
{
  CEGUI::System::getSingleton().setGUISheet(guiRoot);
  }*/

std::string getUserSetting(const std::string& key)
{
  return engine::userSettings[key];
}

void setUserSetting(const std::string& key, const std::string& value)
{
  engine::userSettings[key] = value;
}

static unsigned int stdVectorUnsignedInt_at(std::vector<unsigned int>* self, int index)
{
  return self->at(index);
}

void initLuaBindings()
{
  using namespace luabind;
  //custom bindings
  module(engine::LuaEngine::getSingleton().state)
    [
     class_<std::vector<unsigned int>>("vecUInt")
     .def("at",&stdVectorUnsignedInt_at)
     .def("size",(int(std::vector<unsigned int>::*)(void))&std::vector<unsigned int>::size),
     namespace_("engine")
     [
      class_<engine::GameState, wrap_GameState>("GameState")
      .def(constructor<>())
      .def("enter",&engine::GameState::enter)
      .def("exit",&engine::GameState::exit)
      .def("pause",&engine::GameState::pause)
      .def("resume",&engine::GameState::resume)
      .def("getName",&engine::GameState::getName)
      .def("onTick",&engine::GameState::onTick, &wrap_GameState::default_onTick),
      class_<engine::StateManager>("_StateManager")
      .def("addState",&engine::StateManager::addState)
      .def("switchState",&engine::StateManager::switchState)
      .def("pushState",&engine::StateManager::pushState)
      .def("popState",&engine::StateManager::popState)
      .def("getState",&engine::StateManager::getState)
      .def("onGraphicsSettings",&engine::StateManager::onGraphicsSettings)
      .def_readwrite("isRunning",&engine::StateManager::isRunning),
      def("getUserSetting",&getUserSetting),
      def("setUserSetting",&setUserSetting),
      def("saveSettings",&engine::saveSettings),
      
      class_<engine::Component, wrap_Component>("Component")
      .def(constructor<>())
      .def("getName",&engine::Component::getName)
      .def("registerComponent",&engine::Component::registerComponent),
      /*class_<LuaObjectComponent, engine::Component>("Component")
      .def(constructor<luabind::object>())
      .def(constructor<>())
      .def_readwrite("data",&LuaObjectComponent::object),*/
      class_<engine::Entity>("Entity")
      .def("addComponent",&engine::Entity::addComponent)
      .def("hasComponent",(bool(engine::Entity::*)(const unsigned int) const)&engine::Entity::hasComponent)
      .def("hasComponent",(bool(engine::Entity::*)(const std::string&) const)&engine::Entity::hasComponent)
      .def("getComponent",(engine::Component*(engine::Entity::*)(const unsigned int))&engine::Entity::getComponent)
      .def("getComponent",(engine::Component*(engine::Entity::*)(const std::string&))&engine::Entity::getComponent),
      class_<engine::EntityManager>("EntityManager")
      .def("getEntityID",&engine::EntityManager::getEntityID)
      .def("getEntity",(engine::Entity&(engine::EntityManager::*)(const unsigned int))&engine::EntityManager::getEntity)
      .def("getEntity",(engine::Entity&(engine::EntityManager::*)(const std::string&))&engine::EntityManager::getEntity),
      class_<engine::System, wrap_System>("System")
      .def(constructor<>())
      .def("run",&wrap_System::run)
      .def_readwrite("parent",&engine::System::parent)
      .def_readwrite("entities",&engine::System::entities)
      .def_readwrite("mask",&engine::System::mask),
      class_<engine::World>("World")
      .def(constructor<>())
      .def("onTick",&engine::World::onTick)
      .def("addEntity",&engine::World::addEntity)
      .def("addSystem",&engine::World::addSystem)
      .def("updateEntity",(void(engine::World::*)(const unsigned int))&engine::World::updateEntity)
      .def("updateEntity",(void(engine::World::*)(const std::string&))&engine::World::updateEntity)
      .def_readwrite("mgr",&engine::World::mgr),
      class_<engine::EntityMask>("EntityMask")
      .def(constructor<>())
      .def(self + other<unsigned int>()),
      def("getComponentID",&engine::getComponentID)
      ],
     namespace_("graphics")
     [
      class_<graphics::OgreEngine>("_OgreEngine")
      .def("setupGUIMgr",&graphics::OgreEngine::setupGUIMgr)
      ]
     ];
  globals(engine::LuaEngine::getSingleton().state)["StateManager"] = &(engine::StateManager::getSingleton());
  globals(engine::LuaEngine::getSingleton().state)["OgreEngine"] = &(graphics::OgreEngine::getSingleton());
  //globals(engine::LuaEngine::getSingleton().state)["userSettings"] = &(engine::userSettings);
}
