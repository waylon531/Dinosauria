#include "gui/frame.hpp"

gui::Frame::Frame(float x, float y, float w, float h)
{
  view = std::unique_ptr<graphics::GLViewport>(new graphics::GLViewport(x,y,w,h));
  gui = std::unique_ptr<GUI>(new GUI);
  bbox = graphics::BBox2D(glm::vec2(x,y), w,h);
  gui->addEventWidget(this);
}

gui::Frame::~Frame()
{
  view.reset();
  gui.reset();
}

void gui::Frame::addSubframe(std::shared_ptr<Frame> frame)
{
  subframes.push_back(frame);
  gui->addWidget(frame);
}

void gui::Frame::update(graphics::GLContext* context)
{
  gui->update(context);
}

void gui::Frame::_render()
{
  view->use();
  view->render();
  gui->render();
  glUseProgram(0);
  /*for(std::vector<std::shared_ptr<Frame>>::iterator it=subframes.begin(); it!=subframes.end(); it++)
    {
      (*it)->render();
      }*/
}
