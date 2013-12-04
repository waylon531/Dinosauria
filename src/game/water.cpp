#include "game/water.hpp"

#define SEA_LEVEL 0.0

Ocean::Ocean()
{
#define RES 512
#define TRES 512
  material = std::unique_ptr<graphics::Material>(new graphics::Material("res/materials/water.mat"));
  //hmap = new float[RES*RES*TRES];
  //nmap = new glm::vec3[RES*RES*TRES];
  //tmap = new glm::vec3[RES*RES*TRES];
  //bmap = new glm::vec3[RES*RES*TRES];
  //pgen::cacheWater(RES,TRES,hmap,nmap,tmap,bmap);
  //material->addTexture(std::shared_ptr<graphics::GLTexture3D>(new graphics::GLTexture3D(nmap,RES,TRES,RES,"tex_normal")));
  material->addUniform(graphics::u_time);
  verts = new attrib[4]{(attrib){glm::vec3(-500.0,SEA_LEVEL,-500.0), glm::vec2(0.f,0.f)},
			(attrib){glm::vec3(500.0,SEA_LEVEL,-500.0), glm::vec2(1.f,0.f)},
			(attrib){glm::vec3(-500.0,SEA_LEVEL,500.0), glm::vec2(0.f,1.f)},
			(attrib){glm::vec3(500.0,SEA_LEVEL,500.0), glm::vec2(1.f,1.f)}};
  graphics::GLSLAttributeSet attrs(std::vector<graphics::GLSLAttribute>({graphics::GLSLAttribute("position",3), graphics::GLSLAttribute("texCoord",2)}));
  b_verts = std::unique_ptr<graphics::GLVertexBuffer>(new graphics::GLVertexBuffer(verts, 4, attrs));
  m_model = glm::mat4();

  fbo = std::shared_ptr<graphics::GLFrameBuffer>(new graphics::GLFrameBuffer(W,H, true, "tex_water_depth"));
  pass_color = MKPTR(graphics::GLPass, fbo, 0, "tex_water_color");
  pass_normal = MKPTR(graphics::GLPass, fbo, 1, "tex_water_normal");
  fbo->update();

  fbo_wtex = MKPTR(graphics::GLFrameBuffer, RES,RES,false);
  pass_wtex = MKPTR(graphics::GLPass, fbo_wtex, 0, "tex_normal");
  fbo_wtex->update();
  std::shared_ptr<graphics::GLSL> shader_wtex(new graphics::GLSL("res/shaders/fbo_pass.vert","res/shaders/water_texgen.frag"));
  shader_wtex->attachUniform(graphics::u_time);
  comp_wtex = MKPTR(graphics::Compositor, shader_wtex);
  material->addUniform(pass_wtex->unif);
}

Ocean::~Ocean()
{
  //delete[] hmap;
  //delete[] nmap;
  //delete[] tmap;
  //delete[] bmap;
  material.reset();
  b_verts.reset();
  delete[] verts;
}

void Ocean::render(std::shared_ptr<graphics::World> world)
{
  //draw the texture
  fbo_wtex->use();
  pass_wtex->use();
  comp_wtex->draw();
  fbo_wtex->unuse();
  fbo_wtex->useTex();
  pass_wtex->useTex();
  //draw the scene
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  material->setMatrixProject(m_project);
  material->setMatrixView(m_view);
  material->setMatrixModel(&m_model);
  material->setEyeDir(eye);
  material->setSunDir(sun);
  material->setShadowTex(tex_shadow);
  material->setMatrixLight(m_light);
  material->use();
  b_verts->use();
  fbo->use();
  pass_color->use();
  pass_normal->use();
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  //pass 0
  b_verts->draw(GL_TRIANGLE_STRIP);
  fbo->unuse();
  pass_color->useTex();
  pass_normal->useTex();
  b_verts->unuse();
}
