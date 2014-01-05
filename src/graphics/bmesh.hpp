#pragma once
/** @file graphics/bmesh.hpp Triangle mesh with skeletal animation support */

#include "graphics/opengl.hpp"
#include "graphics/glsl.hpp"
#include "graphics/buffer.hpp"
#include "graphics/ibo.hpp"
#include "graphics/texture.hpp"
#include "graphics/rendObj.hpp"
#include "graphics/material.hpp"

namespace graphics
{

  /** A triangle based mesh
   * This will handle all rendering so you only need to call render() */
  class SkeletalMesh: public RenderableObjectExt
  {
  public:
    
    struct attrib
    {
      glm::vec3 pos;
      glm::vec3 normal;
      glm::vec2 texCoord;
      int boneId[4];
      float boneWeight[4];
    };
    struct Bone
    {
      glm::mat4 offset;
      Bone(glm::mat4 o) : offset(o)
      {
      }
    };
    
  private:

    /** The vertex array */
    attrib* verts;

    /** The index array */
    GLuint* inds;

    /** The bone array */
    std::vector<Bone> bones;

    /** The vertex buffer */
    std::unique_ptr<GLVertexBuffer> b_vert;

    /** The index buffer */
    std::unique_ptr<GLIndexBuffer> b_ind;

    /** The shadow shader */
    std::unique_ptr<GLSL> shader_shadow;
    /** The light matrix uniform */
    std::shared_ptr<GLSLUniform> u_light;

    /** Initialize the shadow shader */
    void initShadow();

  public:

    /** The model matrix */
    glm::mat4 m_model;

    /** Initialize from a file
     * @param fname filename for mesh
     * @param mat material for this mesh */
    SkeletalMesh(const std::string& fname, std::shared_ptr<Material> mat, bool isBin=false);

    /** Initialize from veritces and indices
     * @param v vertex array
     * @param i index array
     * @param nv number of vertices
     * @param ni number of indices */
    //SkeletalMesh(attrib* v, GLuint* i, const int nv, const int ni, std::shared_ptr<Material>);

    /** Destroy */
    ~SkeletalMesh();

    void render();
    void renderShadowPass();
    void setMatrixView(glm::mat4* view);
    void setMatrixProject(glm::mat4* project);
    void setWaterFlag(int* flag);
    
    /** The material of the mesh */
    std::shared_ptr<Material> material;
    
  };
};
