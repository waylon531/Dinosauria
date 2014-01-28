#pragma once
/** @file graphics/mesh.hpp Triangle mesh wrapper for OpenGL */

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
  class Mesh: public RenderableObjectExt
  {
  public:
    
    struct attrib
    {
      glm::vec3 pos;
      glm::vec3 normal;
      glm::vec2 texCoord;
    };
    
  private:

    /** The vertex array */
    attrib* verts;

    /** The index array */
    GLuint* inds;

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
<<<<<<< HEAD
    Mesh(const std::string& fname, std::shared_ptr<Material> mat);
=======
    Mesh(const std::string& fname, std::shared_ptr<Material> mat, bool isBin=false);
>>>>>>> 66660550aeb608a7b118ba234eb0de03df417e5f

    /** Initialize from veritces and indices
     * @param v vertex array
     * @param i index array
     * @param nv number of vertices
     * @param ni number of indices */
    Mesh(attrib* v, GLuint* i, const int nv, const int ni, std::shared_ptr<Material>);

    /** Destroy */
    ~Mesh();

    void render();
    void renderShadowPass();
    void setMatrixView(glm::mat4* view);
    void setMatrixProject(glm::mat4* project);
    void setWaterFlag(int* flag);
    
    /** The material of the mesh */
    std::shared_ptr<Material> material;
    
  };
};
