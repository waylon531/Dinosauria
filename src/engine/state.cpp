#include "engine/state.hpp"

//custom includes
#include "engine/state_manager.hpp"

void engine::GameState::add()
{
  engine::StateManager::getSingleton().addState(this);
}
