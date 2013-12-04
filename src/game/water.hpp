/** @file game/water.hpp */

#include "graphics/buffer.hpp"
#include "graphics/ibo.hpp"
#include "graphics/texture.hpp"
#include "graphics/material.hpp"
#include "graphics/rendObj.hpp"
#include "graphics/world.hpp"
#include "graphics/post.hpp"
#include "game/landscape.hpp"
#include "pgen/water.hpp"

/** A realistic ocean surface simulation */
class Ocean : public graphics::RenderableObjectExt
{
private:

  /** The material for the ocean surface */
  std::unique_ptr<graphics::Material> material;

  struct attrib
  {
    glm::vec3 position;
    glm::vec2 texCoord;
  };
  /** The vertex list */
  attrib* verts;
  /** The vertex buffer */
  std::unique_ptr<graphics::GLVertexBuffer> b_verts;

  glm::mat4 m_model;
  glm::mat4* m_project;
  glm::mat4* m_view;
  glm::mat4* m_light;
  GLint* tex_shadow;
  glm::vec3* sun;
  glm::vec3* eye;

  float* hmap;
  glm::vec3* nmap;
  glm::vec3* tmap;
  glm::vec3* bmap;

  /** The renderer for the water bumpmap texture */
  std::shared_ptr<graphics::GLFrameBuffer> fbo_wtex;
  std::shared_ptr<graphics::GLPass> pass_wtex;
  std::shared_ptr<graphics::Compositor> comp_wtex;

public:

  std::shared_ptr<graphics::GLFrameBuffer> fbo;
  std::shared_ptr<graphics::GLPass> pass_normal;
  std::shared_ptr<graphics::GLPass> pass_color;

  /** Initialize but do not generate */
  Ocean();

  ~Ocean();

  void setMatrixProject(glm::mat4* m){m_project = m;}
  void setMatrixView(glm::mat4* m){m_view = m;}
  void setMatrixLight(glm::mat4* m){m_light = m;}
  void setSunDir(glm::vec3* dir){sun = dir;}
  void setEyeDir(glm::vec3* dir){eye = dir;}
  void setShadowTex(GLint* tex){tex_shadow = tex;}

  /** Render the ocean
   * @param world world to render */
  void render(std::shared_ptr<graphics::World> world);
  /** Render (dummy) */
  void render() {}
};
