#pragma once
/** @file gui/frame.hpp */

#include "gui/widget.hpp"
#include "gui/guiManage.hpp"
#include "graphics/base.hpp"

namespace gui
{

  /** A GUI frame */
  class Frame: public Widget
  {
  private:

    std::shared_ptr<Widget> this_ptr;
    
  protected:
    
    /** The viewport for this frame */
    std::unique_ptr<graphics::GLViewport> view;

    /** The GUI manager for this frame */
    std::unique_ptr<GUI> gui;

    /** The set of subframes */
    std::vector<std::shared_ptr<Frame>> subframes;

    /** Add a new subframe
     * @param frame subframe to add */
    void addSubframe(std::shared_ptr<Frame>);

  public:

    /** Initialize
     * @param x x position of frame
     * @param y y position of frame
     * @param w width of frame
     * @param h height of frame */
    Frame(float,float,float,float);

    /** Destroy */
    ~Frame();

    void update(graphics::GLContext* context);

    void _render();
  };

};
