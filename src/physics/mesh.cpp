#include "physics/mesh.hpp"

physics::Mesh::Mesh()
{
  mesh = new btTriangleMesh;
}

physics::Mesh::~Mesh()
{
  delete mesh;
}

void physics::Mesh::addTriangle(const glm::vec3 v0, const glm::vec3 v1, const glm::vec3 v2)
{
  mesh->addTriangle(btVector3(v0.x,v0.y,v0.z),
		    btVector3(v1.x,v1.y,v1.z),
		    btVector3(v2.x,v2.y,v2.z));
}

void physics::Mesh::buildMesh()
{
  shape = new btBvhTriangleMeshShape(mesh, true, true);
  btDefaultMotionState* fallMotionState =
    //  new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(-scale/2,0,-scale/2)));
    new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
  //fallMotionState->getWorldTransform(trans);
  //trans.setRotation(btQuaternion(0,1,0, 3.f*M_PI));
  //fallMotionState->setWorldTransform(trans);
  btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(0,fallMotionState,shape,btVector3(0,0,0));
  body = new btRigidBody(fallRigidBodyCI);
  //body->setDamping(.98, .95);
}
