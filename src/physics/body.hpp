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

    /** Initialize the body from a shape
     * @param pos position of body */
    void initialize(const glm::vec3& pos);

    /** Get the position in world space
     * @return world space position */
    inline glm::vec3 getPosition() const
    {
      btTransform trans;
      body->getMotionState()->getWorldTransform(trans);
      btVector3 pos = trans.getOrigin();
      return glm::vec3(pos.getX(), pos.getY(), pos.getZ());
    }

    /** Set the rotation as an euler angle
     * @param angle euler angle */
    inline void setRotation(const glm::vec3& angle)
    {
      btTransform trans;
      trans = body->getWorldTransform();
      trans.setRotation(btQuaternion(btVector3(1,0,0),angle.x) * btQuaternion(btVector3(0,1,0),angle.y) * btQuaternion(btVector3(0,0,1),angle.z));
      body->setWorldTransform(trans);
    }

    /** Set the y axis rotation
     * @param angle angle */
    inline void setYRot(const float angle)
    {
      btTransform trans;
      trans = body->getWorldTransform();
      trans.setRotation(btQuaternion(btVector3(0,1,0),angle*M_PI/180.f));
      body->setWorldTransform(trans);
    }
    
    /** Check if object is below water, and if so, reverse gravity */
    void update();

    /** Apply a translational impulse to the rigid body
     * @param v impulse direction */
    void impulseTranslate(const glm::vec3& v);
    
  };
};
