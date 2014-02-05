#include "physics/hull.hpp"

physics::ConvexHull::ConvexHull(std::shared_ptr<graphics::Mesh> mesh, const glm::vec3& pos)
{
}

physics::ConvexHull::ConvexHull(std::shared_ptr<graphics::SkeletalMesh> mesh, const glm::vec3& pos)
{
  shape = new btSphereShape(.5);
  //shape = new btConvexHullShape;
  //for(int i=0; i<mesh->nverts; i++)
  //  {
      //std::cout << "Vertex " << i << " out of " << mesh->nverts << "\r";
  //    ((btConvexHullShape*)shape)->addPoint(btVector3(mesh->verts[i].pos.x, mesh->verts[i].pos.y, mesh->verts[i].pos.z));
  //  }
  btDefaultMotionState* fallMotionState =
    new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(pos.x,pos.y,pos.z)));
  btScalar mass = .5;
  btVector3 fallInertia(0,0,0);
  //shape->calculateLocalInertia(mass,fallInertia);
  btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,shape,fallInertia);
  body = new btRigidBody(fallRigidBodyCI);
  body->setDamping(0.8,.8);
}

physics::ConvexHull::~ConvexHull()
{
}
