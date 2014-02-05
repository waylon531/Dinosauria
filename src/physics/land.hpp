#pragma once

#include "physics/body.hpp"

namespace physics
{

  /** A heightfield collision object */
  class Heightfield: public RigidBody
  {
  public:

    /** Initialize
     * @param hmap heightmap to use
     * @param xres x resolution
     * @param yres y resolution */
    Heightfield(float* hmap, const int xres, const int yres, const float hscale, const float scale);
  };
};
