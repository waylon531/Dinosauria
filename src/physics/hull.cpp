#include "physics/hull.hpp"

physics::ConvexHull::ConvexHull(std::shared_ptr<graphics::Mesh> mesh, const glm::vec3& pos, const float mass)
{
}

physics::ConvexHull::ConvexHull(std::shared_ptr<graphics::SkeletalMesh> mesh, const glm::vec3& pos, const float mass)
{
  //shape = new btConvexHullShape;
  float maxX = 0.f;
  float maxY = 0.f;
  float maxZ = 0.f;
  float minX = 0.f;
  float minY = 0.f;
  float minZ = 0.f;
  
  for(int i=0; i<mesh->nverts; i++)
    {
      //std::cout << "Vertex " << i << " out of " << mesh->nverts << "\r";
      maxX = max(maxX, mesh->verts[i].pos.x);
      maxY = max(maxY, mesh->verts[i].pos.y);
      maxZ = max(maxZ, mesh->verts[i].pos.z);
      
      minX = min(minX, mesh->verts[i].pos.x);
      minY = min(minY, mesh->verts[i].pos.y);
      minZ = min(minZ, mesh->verts[i].pos.z);
  }
  float maxAxis = max(max(maxX-minX, maxY-minY), maxZ-minZ);
  if(maxAxis == maxX-minX)
    {
      shape = new btCapsuleShapeX(max((maxY-minY),(maxZ-minZ))/2, (maxX-minX));
      offset = max((maxY-minY),(maxZ-minZ));
    }
  if(maxAxis == maxY-minY)
    {
      shape = new btCapsuleShape(max((maxX-minX),(maxZ-minZ))/2, (maxY-minY));
      offset = (maxY-minY);
    }
  if(maxAxis == maxZ-minZ)
    {
      shape = new btCapsuleShapeZ(max((maxY-minY),(maxX-minX))/2, (maxZ-minZ));
      offset = max((maxY-minY),(maxX-minX));
    }
  initialize(pos, mass);
}

physics::ConvexHull::~ConvexHull()
{
}
