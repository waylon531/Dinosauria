#include "graphics/world.hpp"

#define SHADOW_W (1024)
#define SHADOW_H SHADOW_W
graphics::World::World(std::shared_ptr<Camera> cam, glm::vec3 sunDir, const bool s) : camera(cam), sun(sunDir), hasShadows(s), isWaterPass(false)
{
  for(int b=0; b<N_SHADOW_BUFFERS; b++)
    {
      b_shadow[b] = std::unique_ptr<GLFrameBuffer>(new GLFrameBuffer(SHADOW_W, SHADOW_H, true, "tex_shadow["+asString(b)+"]"));
      depthTexArray[b] = b_shadow[b]->id_tex_depth;
    }

  fbo = MKPTR(graphics::GLFrameBuffer, W,H, true,"tex_depth");
  pass_color = MKPTR(graphics::GLPass, fbo, 0, "tex_color", GL_RGBA32F, GL_HALF_FLOAT);
  pass_normal = MKPTR(graphics::GLPass, fbo, 1, "tex_normal");
  pass_position = MKPTR(graphics::GLPass, fbo, 2, "tex_position", GL_RGBA32F, GL_HALF_FLOAT);
  pass_params = MKPTR(graphics::GLPass, fbo, 3, "tex_params", GL_RGBA32F, GL_HALF_FLOAT);
  fbo->update();
  shader_deffered = MKPTR(graphics::GLSL, "res/shaders/fbo_pass.vert", "res/shaders/deffered.frag");
  shader_deffered->attachUniform(pass_color->unif);
  shader_deffered->attachUniform(pass_normal->unif);
  shader_deffered->attachUniform(pass_params->unif);
  shader_deffered->attachUniform(pass_position->unif);
  shader_deffered->attachUniform(fbo->unif_depth);
  shader_deffered->attachUniform(MKPTR(graphics::GLSLUniform, "eyeDir", &(cam->dir)));
  shader_deffered->attachUniform(MKPTR(graphics::GLSLUniform, "sunDir", &sun));
  shader_deffered->attachUniform(MKPTR(graphics::GLSLUniform, "m_view", &(cam->mat_view)));
  shader_deffered->attachUniform(MKPTR(graphics::GLSLUniform, "m_project", &(cam->mat_project)));
  for(int b=0; b<N_SHADOW_BUFFERS; b++)
    {
      shader_deffered->attachUniform(std::shared_ptr<GLSLUniform>(new GLSLUniform(("tex_shadow["+asString(b)+"]").c_str(), (GLint*)&depthTexArray[b])));
      shader_deffered->attachUniform(std::shared_ptr<GLSLUniform>(new GLSLUniform(("m_light["+asString(b)+"]").c_str(), &m_light2[b])));
    }
  comp_deffered = MKPTR(graphics::Compositor, shader_deffered);
}

graphics::World::~World()
{
  //  b_shadow.reset();
}

void graphics::World::addMesh(std::shared_ptr<RenderableObjectExt> mesh)
{
  meshs.push_back(mesh);
  mesh->setWaterFlag(&isWaterPass);
}

void graphics::World::removeMesh(std::shared_ptr<RenderableObjectExt> mesh)
{
  for(std::vector<std::shared_ptr<RenderableObjectExt>>::iterator it=meshs.begin(); it!=meshs.end(); it++)
    {
      if((*it).get() == mesh.get())
	{
	  (*it).reset();
	  meshs.erase(it);
	  return;
	}
    }
}

void graphics::World::use(std::shared_ptr<RenderableObjectExt> mesh)
{
      camera->use(mesh);
      mesh->setWaterFlag(&isWaterPass);
      /*mesh->setSunDir(&sun);
      mesh->setShadowTex(depthTexArray);
      mesh->setMatrixLight(m_light2);*/
}

int FALSE = 0;
void graphics::World::render()
{
  //shadow pass
  glm::mat4 m_lightView = glm::lookAt(sun+camera->pos,camera->pos,glm::vec3(0.0,1.0,0.0));
  glm::mat4 m_bias(0.5,0.0,0.0,0.0,
		   0.0,0.5,0.0,0.0,
		   0.0,0.0,0.5,0.0,
		   0.5,0.5,0.5,1.0);
  glm::mat4 m_light[N_SHADOW_BUFFERS];
  float scale = 1.0;
  for(int b=0; b<N_SHADOW_BUFFERS; b++)
    {
      b_shadow[b]->use();
      glClear(GL_DEPTH_BUFFER_BIT);
      //m_lightView = camera->mat_view;
      #define TMP 5
      #define Z_TMP 20
      glm::mat4 m_lightProject = glm::ortho<float>(-TMP*scale,TMP*scale,-TMP*scale,TMP*scale,-Z_TMP,Z_TMP);//glm::mat4(1.0);//glm::ortho<float>(0.0, SHADOW_W, 1.0, SHADOW_H);
      m_light[0] = m_lightProject * m_lightView;
      for(std::vector<std::shared_ptr<RenderableObjectExt>>::iterator it=meshs.begin(); it!=meshs.end(); it++)
	{
	  (*it)->setMatrixView(&m_lightView);
	  (*it)->setMatrixProject(&m_lightProject);
	  (*it)->setWaterFlag(&FALSE);
	  /*(*it)->setEyeDir(&sun);
	  (*it)->setSunDir(&sun);
	  (*it)->setShadowTex(depthTexArray);
	  (*it)->setMatrixLight(m_light);*/
	  (*it)->renderShadowPass();
	}
      m_light2[b] = m_bias * m_light[0];
      b_shadow[b]->unuse();
      scale *= 2.f;
    }
  //color pass
  for(int b=0; b<N_SHADOW_BUFFERS; b++)
    {
      b_shadow[b]->useTex();
    }
  fbo->use();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for(std::vector<std::shared_ptr<RenderableObjectExt>>::iterator it=meshs.begin(); it!=meshs.end(); it++)
    {
      camera->use((*it));
      (*it)->setWaterFlag(&isWaterPass);
      //(*it)->setSunDir(&sun);
      //(*it)->setShadowTex(depthTexArray);
      //(*it)->setMatrixLight(m_light2);
      (*it)->render();
    } 
  fbo->unuse();
  fbo->useTex();
  pass_color->useTex();
  pass_normal->useTex();
  pass_params->useTex();
  pass_position->useTex();
  comp_deffered->draw();
}
