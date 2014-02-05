#pragma once

#include <btBulletDynamicsCommon.h>
#include "util.hpp"

namespace physics
{

  /** The bullet debug drawer class */
  class DebugDraw: public btIDebugDraw
  {
  private:

    struct Line
    {
      btVector3 from;
      btVector3 to;
      btVector3 color;
    };

    std::vector<Line> lines;
    
  public:

    void reportErrorWarning(const char* txt);
    void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&);
    void draw3dText(const btVector3&, const char*);
    void setDebugMode(int mode);
    int getDebugMode() const
    {
      return DBG_DrawWireframe;
    }

    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

    void startFrame();
    void renderFrame();

  };
};
