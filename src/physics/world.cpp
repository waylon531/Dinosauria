#include "physics/world.hpp"

physics::World::World()
{
  broadphase = new btDbvtBroadphase;
  collisionConfig = new btDefaultCollisionConfiguration;
  collisionDispatch = new btCollisionDispatcher(collisionConfig);
  solver = new btSequentialImpulseConstraintSolver;
  world = new btDiscreteDynamicsWorld(collisionDispatch, broadphase, solver, collisionConfig);
  world->setGravity(btVector3(0,-10,0));
}

physics::World::~World()
{
  delete broadphase;
  delete collisionConfig;
  delete collisionDispatch;
  delete solver;
  delete world;
}

void physics::World::addBody(std::shared_ptr<physics::RigidBody> body)
{
  world->addRigidBody(body->body);
}

void physics::World::removeBody(std::shared_ptr<physics::RigidBody> body)
{
  world->removeRigidBody(body->body);
}

void physics::World::step()
{
  world->stepSimulation(1/15.f,20);
  world->synchronizeMotionStates();
}
