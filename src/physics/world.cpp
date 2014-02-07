#include "physics/world.hpp"

physics::World::World()
{
  broadphase = new btDbvtBroadphase;
  collisionConfig = new btDefaultCollisionConfiguration;
  collisionDispatch = new btCollisionDispatcher(collisionConfig);
  solver = new btSequentialImpulseConstraintSolver;
  world = new btDiscreteDynamicsWorld(collisionDispatch, broadphase, solver, collisionConfig);
  world->setGravity(btVector3(0,-10,0));
  //world->getDispatchInfo().m_useContinuous = true;
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
  bodies.push_back(body);
}

void physics::World::removeBody(std::shared_ptr<physics::RigidBody> body)
{
  world->removeRigidBody(body->body);
  for(int i=0; i<bodies.size(); i++)
    {
      if(bodies[i].get() == body.get())
	{
	  bodies.erase(bodies.begin()+i);
	  return;
	}
    }
}

void physics::World::step()
{
  world->stepSimulation(1/60.f,10);
  world->synchronizeMotionStates();
  for(int i=0; i<bodies.size(); i++)
    {
      bodies[i]->update();
    }
}
