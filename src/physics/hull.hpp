#pragma once

#include "physics/body.hpp"
#include "graphics/mesh.hpp"
#include "graphics/bmesh.hpp"

namespace physics
{

  /** A convex hull */
  class ConvexHull: public RigidBody
  {
  public:

    /** Initialize from a static mesh
     * @param mesh mesh to use
     * @param pos position to initialize in */
    ConvexHull(std::shared_ptr<graphics::Mesh> mesh, const glm::vec3& pos);

    /** Initialize from a skinned mesh
     * @param mesh mesh to use */
    ConvexHull(std::shared_ptr<graphics::SkeletalMesh> mesh, const glm::vec3& pos);

    ~ConvexHull();
  };
};
