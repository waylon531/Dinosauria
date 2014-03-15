//file: engine/state_manager.hpp
//purpose: Singleton class to handle all game states

#pragma once

//external includes
#include <OISKeyboard.h>
#include <OISMouse.h>
//custom includes
#include "engine/state.hpp"

namespace engine
{

  /** A singleton manager for switching game states */
  class StateManager
  {
  private:

    /** All available states */
    std::map<std::string, GameState*> states;

    /** The state stack */
    std::vector<GameState*> stack;
    
  public:

    /** The current state being run */
    GameState* currentState;

    /** Get a singleton of this class
     * @return singleton instance */
    static StateManager& getSingleton()
    {
      static StateManager mgr;
      return mgr;
    }

    /** Whether the state manager needs to be running */
    bool isRunning =false;

    /** Begin the main loop */
    void start();

    /** Add a state to the manager
     * @param state state to add */
    void addState(GameState* state);

    /** Switch states to a particular state
     * @param name name of state to switch to */
    void switchState(const std::string& name);

    /** Push current state on to stack and switch to a new one
     * @param name name of state to switch to */
    void pushState(const std::string& name);

    /** Pop current state off stack */
    void popState();

    /** Get a state by name
     * @param name name of state to get
     * @return state with name _name_ */
    GameState* getState(const std::string& name);

    /** Handle a keydown event
     * @param e event */
    void onKeydown(const OIS::KeyEvent& e);
    
    /** Handle a keyup event
     * @param e event */
    void onKeyup(const OIS::KeyEvent& e);
    
    /** Handle a mouse movement event
     * @param e event */
    void onMouseMove(const OIS::MouseEvent& e);
    
    /** Handle a mouse press event
     * @param e event
     * @param id mouse button id */
    void onMouseDown(const OIS::MouseEvent& e, OIS::MouseButtonID id);
    
    /** Handle a mouse release event
     * @param e event
     * @param id mouse button id */
    void onMouseUp(const OIS::MouseEvent& e, OIS::MouseButtonID id);

    /** Handle a time tick event
     * @param pulse time since last frame */
    void onTick(const float pulse);

    /** Handle a change in the graphics settings */
    void onGraphicsSettings();

  };
};
