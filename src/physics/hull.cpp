#include "physics/hull.hpp"

physics::ConvexHull::ConvexHull(std::shared_ptr<graphics::Mesh> mesh, const glm::vec3& pos, const float mass)
{
  shape = new btConvexHullShape;
  
  for(int i=0; i<mesh->nverts; i++)
    {
      //std::cout << "Vertex " << i << " out of " << mesh->nverts << "\r";
      ((btConvexHullShape*)shape)->addPoint(btVector3(mesh->verts[i].pos.x, mesh->verts[i].pos.y, mesh->verts[i].pos.z));
    }
  initialize(pos, mass);
}

physics::ConvexHull::ConvexHull(std::shared_ptr<graphics::SkeletalMesh> mesh, const glm::vec3& pos, const float mass)
{
  shape = new btConvexHullShape;
  
  for(int i=0; i<mesh->nverts; i++)
    {
      //std::cout << "Vertex " << i << " out of " << mesh->nverts << "\r";
      ((btConvexHullShape*)shape)->addPoint(btVector3(mesh->verts[i].pos.x, mesh->verts[i].pos.y, mesh->verts[i].pos.z));
    }
  initialize(pos, mass);
}

physics::ConvexHull::~ConvexHull()
{
}
