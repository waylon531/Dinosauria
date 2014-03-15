//file: engine/state.hpp
//purpose: Single game state

#pragma once

//external includes
#include <OISKeyboard.h>
#include <OISMouse.h>

namespace engine
{
  /** An abstract game state class */
  class GameState
  {
  public:

    /** Get the string ID of this state
     * @return string name of this state */
    virtual std::string getName() const =0;
    
    /** Enter this state as a new state */
    virtual void enter() =0;
    
    /** Exit this state without intention of resuming */
    virtual void exit() =0;
    
    /** Pause this state when being placed on the stack */
    virtual void pause() =0;
    
    /** Resume this state off the stack */
    virtual void resume() =0;
    
    /** Handle a keydown event
     * @param e event */
    virtual void onKeydown(const OIS::KeyEvent& e)
    {
    }
    
    /** Handle a keyup event
     * @param e event */
    virtual void onKeyup(const OIS::KeyEvent& e)
    {
    }
    
    /** Handle a mouse movement event
     * @param e event */
    virtual void onMouseMove(const OIS::MouseEvent& e)
    {
    }
    
    /** Handle a mouse press event
     * @param e event
     * @param id mouse button id */
    virtual void onMouseDown(const OIS::MouseEvent& e, OIS::MouseButtonID id)
    {
    }
    
    /** Handle a mouse release event
     * @param e event
     * @param id mouse button id */
    virtual void onMouseUp(const OIS::MouseEvent& e, OIS::MouseButtonID id)
    {
    }

    /** Handle a time tick event
     * @param pulse amount of time since last tick */
    virtual void onTick(const float pulse)
    {
    }

    /** Handle a change in the graphics settings */
    virtual void onGraphicsSettings()
    {
    }
    
  };
};
