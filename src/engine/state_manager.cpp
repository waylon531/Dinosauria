#include "engine/state_manager.hpp"

void engine::StateManager::start()
{
  isRunning = true;
  while(isRunning)
    {
    }
}

void engine::StateManager::addState(GameState* state)
{
  states[state->getName()] = state;
}

void engine::StateManager::switchState(const std::string& name)
{
  if(currentState)
    {
      currentState->exit();
    }
  currentState = states.at(name);
  currentState->enter();
}

void engine::StateManager::pushState(const std::string& name)
{
  currentState->pause();
  stack.push_back(currentState);
  currentState = states.at(name);
  currentState->enter();
}

void engine::StateManager::popState()
{
  currentState->exit();
  currentState = stack[stack.size()-1];
  stack.pop_back();
  currentState->resume();
}

engine::GameState* engine::StateManager::getState(const std::string& name)
{
  return states.at(name);
}

void engine::StateManager::onKeyup(const OIS::KeyEvent& e)
{
  currentState->onKeyup(e);
}

void engine::StateManager::onKeydown(const OIS::KeyEvent& e)
{
  currentState->onKeydown(e);
}

void engine::StateManager::onMouseMove(const OIS::MouseEvent& e)
{
  currentState->onMouseMove(e);
}

void engine::StateManager::onMouseDown(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
  currentState->onMouseDown(e,id);
}

void engine::StateManager::onMouseUp(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
  currentState->onMouseUp(e,id);
}

void engine::StateManager::onTick(const float pulse)
{
  currentState->onTick(pulse);
}

void engine::StateManager::onGraphicsSettings()
{
  for(std::map<std::string, GameState*>::iterator it=states.begin(); it!=states.end(); it++)
    {
      (*it).second->onGraphicsSettings();
    }
}
