/** @file physics/world.hpp A bullet wrapper */
#pragma once

#include <btBulletDynamicsCommon.h>
#include "physics/body.hpp"

namespace physics
{
  /** A bullet physics simulation world */
  class World
  {
  private:

    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* collisionDispatch;
    btSequentialImpulseConstraintSolver* solver;

  public:

    btDynamicsWorld* world;

    /** The body list */
    std::vector<std::shared_ptr<RigidBody>> bodies;

    /** Initialize */
    World();

    /** Destroy */
    ~World();

    /** Add a new rigid body to the world
     * @param body body to add */
    void addBody(std::shared_ptr<RigidBody> body);

    /** Remove a rigid body from the world
     * @param body body to remove */
    void removeBody(std::shared_ptr<RigidBody> body);

    /** Step the simulation */
    void step();
    
  };
};
