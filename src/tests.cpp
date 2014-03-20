//file: tests.cpp
//purpose: store all unit tests

//#define BOOST_TEST_DYN_LINK
//#define BOOST_TEST_MODULE otst
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "util.hpp"
#include "engine/state_manager.hpp"
#include "engine/state.hpp"
#include "engine/component.hpp"
#include "engine/entity.hpp"
#include "engine/entity_mgr.hpp"
#include "engine/world.hpp"
#include "engine/system.hpp"
#include "scripting/context.hpp"

BOOST_AUTO_TEST_SUITE(Utility)

BOOST_AUTO_TEST_CASE(Lerp)
{
  BOOST_CHECK_EQUAL(lerp(0.f,1.f,0.5f), 0.5f);
  BOOST_CHECK_EQUAL(lerp(0.f,1.f,0.f), 0.f);
  BOOST_CHECK_EQUAL(lerp(0.f,2.f,0.5f), 1.f);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE()

class TestState: public engine::GameState
{
  enum Moves {MOVE_ENTER, MOVE_EXIT, MOVE_PAUSE, MOVE_RESUME};
  int move;
  
  void enter()
  {
    move = MOVE_ENTER;
  }

  void exit()
  {
    move = MOVE_EXIT;
  }

  void pause()
  {
    move = MOVE_PAUSE;
  }

  void resume()
  {
    move = MOVE_RESUME;
  }

  std::string getName() const
  {
    return "test";
  }
};

class TestState2: public engine::GameState
{
  enum Moves {MOVE_ENTER, MOVE_EXIT, MOVE_PAUSE, MOVE_RESUME};
  int move;
  
  void enter()
  {
    move = MOVE_ENTER;
  }

  void exit()
  {
    move = MOVE_EXIT;
  }

  void pause()
  {
    move = MOVE_PAUSE;
  }

  void resume()
  {
    move = MOVE_RESUME;
  }

  std::string getName() const
  {
    return "test2";
  }
};

BOOST_AUTO_TEST_CASE(addState)
{
  BOOST_REQUIRE_NO_THROW(engine::StateManager::getSingleton().addState(new TestState));
  BOOST_REQUIRE_NO_THROW(engine::StateManager::getSingleton().addState(new TestState2));
  //engine::StateManager::getSingleton().getState("test");
}

BOOST_AUTO_TEST_CASE(switchState)
{
  BOOST_REQUIRE_NO_THROW(engine::StateManager::getSingleton().switchState("test"));
  BOOST_CHECK_EQUAL(engine::StateManager::getSingleton().currentState->getName(), "test");
}

BOOST_AUTO_TEST_CASE(popState)
{
  engine::StateManager::getSingleton().pushState("test2");
  BOOST_CHECK_EQUAL(engine::StateManager::getSingleton().currentState->getName(), "test2");
  engine::StateManager::getSingleton().popState();
  BOOST_CHECK_EQUAL(engine::StateManager::getSingleton().currentState->getName(), "test");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Scripting)

BOOST_AUTO_TEST_CASE(start)
{
  BOOST_CHECK_NO_THROW(engine::LuaEngine::getSingleton().startup(false));
}

BOOST_AUTO_TEST_CASE(stop)
{
  BOOST_CHECK_NO_THROW(engine::LuaEngine::getSingleton().shutdown());
}

BOOST_AUTO_TEST_SUITE_END()

class TestComponent: public engine::Component
{
public:

  int x;

  TestComponent()
  {
  }

  TestComponent(const int y) : x(y)
  {
  }

  std::string getName() const
  {
    return "test";
  }
};

class TestComponent2: public engine::Component
{
public:

  int x;

  std::string getName() const
  {
    return "test2";
  }
};

class TestComponent3: public engine::Component
{
public:

  int x;

  std::string getName() const
  {
    return "test3";
  }
};

BOOST_AUTO_TEST_SUITE(Component)
  
BOOST_AUTO_TEST_CASE(registerComponent)
{
  TestComponent* test = new TestComponent;
  test->registerComponent();
  (new TestComponent2)->registerComponent();
  (new TestComponent3)->registerComponent();
  BOOST_CHECK_EQUAL(test->getID(), engine::getComponentID("test"));
  delete test;
}

BOOST_AUTO_TEST_SUITE_END()

struct F0
{
  engine::EntityManager* entityMgr;
  F0()
  {
    entityMgr = new engine::EntityManager;
  }
  ~F0()
  {
    delete entityMgr;
  }
};

BOOST_AUTO_TEST_SUITE(Entity_and_EntityManager)

F0 f0;

BOOST_AUTO_TEST_CASE(addEntity)
{
  BOOST_CHECK_NO_THROW(f0.entityMgr->addEntity("test"));
}

BOOST_AUTO_TEST_CASE(getEntityID)
{
  BOOST_CHECK_EQUAL(f0.entityMgr->getEntityID("test"), 0);
}

BOOST_AUTO_TEST_CASE(getEntity)
{
  BOOST_CHECK_EQUAL(f0.entityMgr->getEntity("test").id, 0);
  BOOST_CHECK_EQUAL(f0.entityMgr->getEntity("test").name, "test");
}

BOOST_AUTO_TEST_CASE(addComponent)
{
  BOOST_CHECK_NO_THROW(f0.entityMgr->getEntity("test").addComponent(new TestComponent(5)));
  BOOST_CHECK_NO_THROW(f0.entityMgr->getEntity("test").addComponent(new TestComponent2));
}

BOOST_AUTO_TEST_CASE(hasComponent)
{
  BOOST_CHECK(f0.entityMgr->getEntity("test").hasComponent("test"));
  BOOST_CHECK(f0.entityMgr->getEntity("test").hasComponent("test2"));
}

BOOST_AUTO_TEST_CASE(getComponent)
{
  BOOST_CHECK_EQUAL(f0.entityMgr->getEntity("test").getComponent_<TestComponent>()->x, 5);
}

BOOST_AUTO_TEST_CASE(removeComponent)
{
  engine::Entity& e_test = f0.entityMgr->getEntity("test");
  BOOST_CHECK_NO_THROW(e_test.removeComponent("test"));
  BOOST_CHECK(!e_test.hasComponent("test"));
}

BOOST_AUTO_TEST_CASE(matchesMask)
{
  engine::EntityMask mask;
  mask << engine::getComponentID("test");
  BOOST_CHECK(f0.entityMgr->getEntity("test").matchesMask(mask));
  mask << engine::getComponentID("test3");
  BOOST_CHECK(!f0.entityMgr->getEntity("test").matchesMask(mask));
  mask = 0x0;
  mask << engine::getComponentID("test") << engine::getComponentID("test2");
  BOOST_CHECK(f0.entityMgr->getEntity("test").matchesMask(mask));
}

BOOST_AUTO_TEST_CASE(removeEntity)
{
  BOOST_CHECK_NO_THROW(f0.entityMgr->removeEntity("test"));
  BOOST_CHECK_THROW(f0.entityMgr->getEntity("test"), std::out_of_range);
}

  
BOOST_AUTO_TEST_SUITE_END()

class TestSystem: public engine::System
{
public:

  TestSystem()
  {
    mask = engine::EntityMask() << engine::getComponentID("test");
  }

  void run(const float pulse)
  {
    for(int i=0; i<entities.size(); i++)
      {
	parent->mgr->getEntity(entities[i]).getComponent_<TestComponent>()->x++;
      }
  }
};

BOOST_AUTO_TEST_SUITE(World_and_System)

engine::World world;

BOOST_AUTO_TEST_CASE(addSystem)
{
  BOOST_REQUIRE_NO_THROW(world.addSystem(new TestSystem));
}

BOOST_AUTO_TEST_CASE(addEntity)
{
  engine::Entity& e = world.addEntity("test");
  e.addComponent(new TestComponent(1));
  world.updateEntity("test");
  
  engine::Entity& e2 = world.addEntity("test2");
  world.updateEntity("test2");
}

BOOST_AUTO_TEST_CASE(run)
{
  BOOST_REQUIRE_NO_THROW(world.onTick(0.0));
  BOOST_CHECK_EQUAL(world.mgr->getEntity("test").getComponent_<TestComponent>()->x, 2);
}
  
BOOST_AUTO_TEST_SUITE_END()
