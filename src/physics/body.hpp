#pragma once

#include <btBulletDynamicsCommon.h>
#include "graphics/opengl.hpp"
#include "util.hpp"

namespace physics
{
  /** A rigid body wrapper */
  class RigidBody
  {
  public:

    /** The object's shape */
    btCollisionShape* shape;

    /** The rigid body object */
    btRigidBody* body;

    /** Destroy */
    ~RigidBody();

    /** Get the position in world space
     * @return world space position */
    inline glm::vec3 getPosition() const
    {
      btTransform trans;
      body->getMotionState()->getWorldTransform(trans);
      btVector3 pos = trans.getOrigin();
      return glm::vec3(pos.getX(), pos.getY(), pos.getZ());
    }

    /** Apply a translational impulse to the rigid body
     * @param v impulse direction */
    void impulseTranslate(const glm::vec3& v);
    
  };
};
