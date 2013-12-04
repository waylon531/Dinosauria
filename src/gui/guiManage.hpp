#pragma once
/** @file gui/guiManage.hpp GUI manager */

#include "gui/widget.hpp"
#include "graphics/base.hpp"
#include "graphics/types.hpp"

namespace gui
{
  /** A GUI manager */
  class GUI: public graphics::RenderableObject
  {
  private:

    /** A list of widgets */
    std::vector<std::shared_ptr<Widget>> widgets;

    /** A list of event widgets */
    std::vector<Widget*> ewidgets;    
  public:

    /** Construct */
    GUI();

    /** Destroy */
    ~GUI();

    void render();

    /** Add a new widget
     * @param widget widget to add */
    void addWidget(std::shared_ptr<Widget> widget);
    /** Add a new invisible widget
     * @param widget widget to add */
    void addEventWidget(Widget* widget);

    /** Update and handle events
     * @param context OpenGL context object */
    void update(graphics::GLContext* context);
  };
};
