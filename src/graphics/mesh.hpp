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

  /** A 3D bounding box */
  class BBox3D
  {
  public:

    /** Lowest point on box */
    glm::vec3 low;

    /** Highest point on box */
    glm::vec3 high;

    /** Initialize
     * @param lo low point
     * @param hi high point */
    BBox3D(const glm::vec3& lo, const glm::vec3& hi) : low(lo), high(hi)
    {
    }

    BBox3D()
    {
      low = glm::vec3(INFINITY,INFINITY,INFINITY);
      high = glm::vec3(-INFINITY,-INFINITY,-INFINITY);
    }

    /** Get the union with a point
     * @param p point */
    inline BBox3D getUnion(const glm::vec3& p) const
    {
      return BBox3D(glm::vec3(min(low.x,p.x),
			      min(low.y,p.y),
			      min(low.z,p.z)),
		    glm::vec3(max(high.x,p.x),
			      max(high.y,p.y),
			      max(high.z,p.z)));
    }

    /** Check if a point is in the bounding box
     * @param p point */
    inline bool inside(const glm::vec3& p) const
    {
      return (low.x < p.x) &&
	(low.y < p.y) &&
	(low.z < p.z) &&
	(high.z > p.z) &&
	(high.y > p.y) &&
	(high.z > p.z);
    }
  };

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
      glm::vec3 tangent;
    };
    
  private:

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

    /** Check if this object shoudl be culled */
    bool getCulling();

  public:

    /** The number of vertices */
    int nverts;

    /** The vertex array */
    attrib* verts;

    /** The index array */
    GLuint* inds;

    /** The model matrix */
    glm::mat4 m_model;

    glm::mat4* m_view;
    glm::mat4* m_project;

    /** The object space bounding box */
    BBox3D bbox;

    /** Initialize from a file
     * @param fname filename for mesh
     * @param mat material for this mesh */
    Mesh(const std::string& fname, std::shared_ptr<Material> mat, bool isBin=false);

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
