#include "physics/debug.hpp"
#include "graphics/opengl.hpp"
#include <iostream>

void physics::DebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
  lines.push_back((Line){from, to, color});
}


void physics::DebugDraw::reportErrorWarning(const char* txt)
{
  std::cout << txt << std::endl;
}

void physics::DebugDraw::drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&)
{
}

void physics::DebugDraw::draw3dText(const btVector3&, const char*)
{
}

void physics::DebugDraw::setDebugMode(int mode)
{
}

void physics::DebugDraw::startFrame()
{
  lines.clear();
}

void physics::DebugDraw::renderFrame()
{
  glUseProgram(0);


  glDisable(GL_LIGHTING);
  glDisable(GL_BLEND);
  glBegin(GL_LINES);
  for(int i=0; i<lines.size(); i++)
    {
      glColor3f(lines[i].color.getX(), lines[i].color.getY(), lines[i].color.getZ());
      glVertex3f(lines[i].from.getX(),lines[i].from.getY(),lines[i].from.getZ());
      glVertex3f(lines[i].to.getX(),lines[i].to.getY(),lines[i].to.getZ());
    }
  glEnd();
}
