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
     * @param pos position of body
     * @param mass mass of body */
    void initialize(const glm::vec3& pos, const float mass);

    /** Get the position in world space
     * @return world space position */
    inline glm::vec3 getPosition() const
    {
      btTransform trans;
      body->getMotionState()->getWorldTransform(trans);
      btVector3 pos = trans.getOrigin();
      return glm::vec3(pos.getX(), pos.getY(), pos.getZ());
    }

    /** Get euler angle rotation
     * @return rotation of object as euler angle */
    inline glm::vec3 getRotation() const
    {
      btTransform trans;
      body->getMotionState()->getWorldTransform(trans);
      btQuaternion quat = trans.getRotation();
      float w=quat.getW(); float x=quat.getX(); float y=quat.getY(); float z=quat.getZ();
      double sqw = w*w; double sqx = x*x; double sqy = y*y; double sqz = z*z;
      glm::vec3 euler;
      euler.z = ((atan2(2.0 * (x*y + z*w),(sqx - sqy - sqz + sqw))));
      euler.x = ((atan2(2.0 * (y*z + x*w),(-sqx - sqy + sqz + sqw))));
      euler.y = ((asin(-2.0 * (x*z - y*w))));
      return euler;
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
      trans.setRotation(btQuaternion(btVector3(0,1,0),angle));
      body->setWorldTransform(trans);
    }
    
    /** Check if object is below water, and if so, reverse gravity */
    void update();

    /** Apply a translational impulse to the rigid body
     * @param v impulse direction */
    void impulseTranslate(const glm::vec3& v);

    /*** Apply a angular impulse to the rigid body
     * @param v euler angle to rotate */
    void impulseRotate(const glm::vec3& v);
    
  };
};
