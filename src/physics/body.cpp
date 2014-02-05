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
