#pragma once

#include "physics/body.hpp"

namespace physics
{

  /** A triangle mesh collision object */
  class Mesh: public RigidBody
  {
  private:

    btTriangleMesh* mesh;
    
  public:

    /** Initialize */
    Mesh();

    /** Destroy */
    ~Mesh();

    /** Add a triangle
     * @param v0 vertex 0
     * @param v1 vertex 1
     * @param v2 vertex 2 */
    void addTriangle(const glm::vec3 v0, const glm::vec3 v1, const glm::vec3 v2);

    /** Build rigid body structure */
    void buildMesh();
    
  };
};
