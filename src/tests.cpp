//file: tests.cpp
//purpose: store all unit tests

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE otst
#include <boost/test/unit_test.hpp>

#include "util.hpp"
#include "engine/state_manager.hpp"
#include "engine/state.hpp"

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
  engine::StateManager::getSingleton().addState(new TestState);
  engine::StateManager::getSingleton().addState(new TestState2);
  engine::StateManager::getSingleton().getState("test");
}

BOOST_AUTO_TEST_CASE(switchState)
{
  engine::StateManager::getSingleton().switchState("test");
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
