#pragma once
/** @file gui/widget.hpp Abstract GUI widget interface */

#include "util.hpp"
#include "graphics/rendObj.hpp"
#include "graphics/opengl.hpp"
#include "graphics/types.hpp"

namespace gui
{

  /** An abstract GUI widget */
  class Widget: public graphics::RenderableObject
  {
  public:

    /** The widget's bounding box */
    graphics::BBox2D bbox;
    
    /** The widget's position on the screen */
    glm::vec2 position;

    bool e_mouseOver;
    bool e_mouseClick;

    Widget()
    {
    }
    
    /** Initialize from a position
     * @param pos position */
    Widget(const glm::vec2& pos) : position(pos)
    {
      e_mouseOver=false;
      e_mouseClick=false;
    }

    virtual void render() =0;

    /** Mouse over callback */
    virtual void onMouseOver(const glm::vec2& pos)
    {
    }
    /** Callback for when the mouse is not on the widget */
    virtual void onNotMouseOver()
    {
    }
    /** Callback for when it is clicked */
    virtual void onMouseClick(const glm::vec2& pos)
    {
    }
    /** Callback for when it is released */
    virtual void onMouseRelease(const glm::vec2& pos)
    {
    }
    /** Callback for checking if keys are held */
    virtual void onKey(GLFWwindow* window)
    {
    }

    void mouseOver(const glm::vec2& pos)
    {
      if(e_mouseOver==false)
	{
	  e_mouseOver=true;
	  onMouseOver(pos);
	}
    }
    void notMouseOver()
    {
      if(e_mouseOver==true)
	{
	  e_mouseOver=false;
	  onNotMouseOver();
	}
    }
    void mouseClick(const glm::vec2& pos)
    {
      if(e_mouseClick==false)
	{
	  e_mouseClick=true;
	  onMouseClick(pos);
	}
    }
    void mouseRelease(const glm::vec2& pos)
    {
      if(e_mouseClick==true)
	{
	  e_mouseClick=false;
	  onMouseRelease(pos);
	}
    }
  };
  
};
