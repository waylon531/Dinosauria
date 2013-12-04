#include "gui/guiManage.hpp"

gui::GUI::GUI()
{
}

gui::GUI::~GUI()
{
}

void gui::GUI::addWidget(std::shared_ptr<Widget> widget)
{
  widgets.push_back(widget);
  ewidgets.push_back(widget.get());
}
void gui::GUI::addEventWidget(Widget* widget)
{
  ewidgets.push_back(widget);
}

void gui::GUI::update(graphics::GLContext* context)
{
  glm::vec2 mousePos;
  double x,y;
  glfwGetCursorPos(context->getWindow(), &x, &y);
  mousePos.x = x;
  mousePos.y = H-y;
  bool mousePress = glfwGetMouseButton(context->getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
  //std::cout << mousePos.x << "," << mousePos.y << std::endl;
  for(std::vector<Widget*>::iterator it=ewidgets.begin(); it!=ewidgets.end(); it++)
    {
      //check for onMouseOver
      if((*it)->bbox.inside(mousePos))
	{
	  (*it)->mouseOver(mousePos);
	  if(mousePress)
	    {
	      (*it)->mouseClick(mousePos);
	    }
	  else
	    {
	      (*it)->mouseRelease(mousePos);
	    }
	}
      else
	{
	  (*it)->notMouseOver();
	  (*it)->mouseRelease(mousePos);
	}
      (*it)->onKey(context->getWindow());
    }
}

void gui::GUI::render()
{
  for(std::vector<std::shared_ptr<Widget>>::iterator it=widgets.begin(); it!=widgets.end(); it++)
    {
      (*it)->render();
    }
}
