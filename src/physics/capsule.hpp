#pragma once

#include "physics/body.hpp"
#include "graphics/mesh.hpp"
#include "graphics/bmesh.hpp"

namespace physics
{

  /** A mesh based capsule */
  class Capsule: public RigidBody
  {
  public:

    /** The height of the capsule */
    float offset;

    /** Initialize from a static mesh
     * @param mesh mesh to use
     * @param pos position to initialize in */
    Capsule(std::shared_ptr<graphics::Mesh> mesh, const glm::vec3& pos, const float mass);

    /** Initialize from a skinned mesh
     * @param mesh mesh to use */
    Capsule(std::shared_ptr<graphics::SkeletalMesh> mesh, const glm::vec3& pos, const float mass);

    ~Capsule();
  };
};
