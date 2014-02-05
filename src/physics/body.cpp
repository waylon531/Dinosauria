#include "physics/body.hpp"

physics::RigidBody::~RigidBody()
{
  delete shape;
  delete body;
}

void physics::RigidBody::impulseTranslate(const glm::vec3& v)
{
  body->applyCentralImpulse(btVector3(v.x,v.y,v.z));
  //body->setLinearVelocity(body->getLinearVelocity()+btVector3(v.x,v.y,v.z));
}

void physics::RigidBody::initialize(const glm::vec3& pos)
{ 
  btDefaultMotionState* fallMotionState =
    new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(pos.x,pos.y,pos.z)));
  btScalar mass = .5;
  btVector3 fallInertia(0,0,0);
  //shape->calculateLocalInertia(mass,fallInertia);
  btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,shape,fallInertia);
  body = new btRigidBody(fallRigidBodyCI);
  body->setDamping(0.8,.8);
}

void physics::RigidBody::update()
{
  if(getPosition().y < 0.f)
    {
      body->setGravity(btVector3(0,-4,0));
    }
  else
    {
      body->setGravity(btVector3(0,-10,0));
    }
}
