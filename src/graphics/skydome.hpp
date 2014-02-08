#pragma once
/** @file graphics/skydome.hpp */

#include "graphics/mesh.hpp"

namespace graphics
{

  /** A dynamic glsl skydome */
  class Skydome: public RenderableObjectExt
  {
  public:

    /** The material */
    std::shared_ptr<Material> material;

    /** The mesh */
    std::shared_ptr<Mesh> mesh;

    /** Initialize */
    Skydome();

    void setMatrixView(glm::mat4* m)
    {
      mesh->setMatrixView(m);
    }

    void setMatrixProject(glm::mat4* m)
    {
      mesh->setMatrixProject(m);
    }

    void setWaterFlag(int* flag)
    {
      mesh->setWaterFlag(flag);
    }
    
    void render();
    void renderShadowPass();

  };
};
