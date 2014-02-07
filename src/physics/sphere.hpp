#pragma once

#include "physics/body.hpp"

namespace physics
{

  /** A spherical rigid body */
  class Sphere: public RigidBody
  {
  public:

    /** Initialize
     * @param r radius of sphere */
    Sphere(const float radius, const glm::vec3& pos, const float mass);
  };
};
