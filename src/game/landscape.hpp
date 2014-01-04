#pragma once
/** @file game/landscape.hpp */

#include "graphics/mesh.hpp"
#include "graphics/texture.hpp"
#include "pgen/terrain.hpp"

/** A landscape controller */
class Landscape: public graphics::RenderableObjectExt
{
private:

  std::unique_ptr<graphics::Material> material;
  std::unique_ptr<graphics::Material> material_shadow;

  glm::mat4 m_model;
  struct attrib
  {
    glm::vec3 pos;
    glm::vec2 texCoord;
    glm::vec3 normal;
  };
  attrib* verts;
  GLuint* inds;
  std::unique_ptr<graphics::GLVertexBuffer> b_verts;
  std::unique_ptr<graphics::GLIndexBuffer> b_inds;
  
  float* hmap;
  glm::vec3* tmap;
  glm::vec3* btmap;
  float* bmap;
  glm::vec3* nmap;
  std::shared_ptr<graphics::GLTexture2D> tex_height;
  std::shared_ptr<graphics::GLTexture2D> tex_b;
  std::shared_ptr<graphics::GLTexture2D> tex_normal;

  bool hasLoaded;

  /** Initialize the mesh */
  void initMesh(float* value, Callback& callback);

  /** Maximum tesselation level */
  GLint tessMax;
  
public:

  /** The view matrix to use for tesselation during the shadow pass */
  glm::mat4* m_tessView;

  /** The wireframe rendering flag */
  GLuint isWireframe;

  /** Initialize */
  Landscape();

  /** Generate a landscape */
  void generate(float* value, Callback& callback);

  /** Load from a file
   * @param fname file name */
  void loadMap(const std::string& fname, float*,Callback&);

  /** Reset the landscape before loading */
  void reset();
  
  ~Landscape();

  /** Evaluate the terrain height at a location
   * @param x x coordinate
   * @param y y coordinate
   * @return terrain height at (x,y) */
  float eval(const float x, const float y) const;

  void setMatrixView(glm::mat4* view) {material->setMatrixView(view); material_shadow->setMatrixView(view);}
  void setMatrixProject(glm::mat4* project) {material->setMatrixProject(project); material_shadow->setMatrixProject(project);}
  void setWaterFlag(int* wflag) {material->setWaterFlag(wflag); material_shadow->setWaterFlag(wflag);}
  void renderShadowPass();
  void render();

  /** Save to a file
   * @param fname file name */
  void save(const std::string& fname);
};
