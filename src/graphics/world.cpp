#include "graphics/world.hpp"

#define SHADOW_W (1024)
#define SHADOW_H SHADOW_W
graphics::World::World(std::shared_ptr<Camera> cam, glm::vec3 sunDir, const bool s) : camera(cam), sun(sunDir), hasShadows(s)
{
  for(int b=0; b<N_SHADOW_BUFFERS; b++)
    {
      b_shadow[b] = std::unique_ptr<GLFrameBuffer>(new GLFrameBuffer(SHADOW_W, SHADOW_H, true, "tex_shadow["+asString(b)+"]"));
      depthTexArray[b] = b_shadow[b]->id_tex_depth;
    }
}

graphics::World::~World()
{
  //  b_shadow.reset();
}

void graphics::World::addMesh(std::shared_ptr<RenderableObjectExt> mesh)
{
  meshs.push_back(mesh);
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
      mesh->setSunDir(&sun);
      mesh->setShadowTex(depthTexArray);
      mesh->setMatrixLight(m_light2);
}

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
      #define TMP 10
      glm::mat4 m_lightProject = glm::ortho<float>(-TMP*scale,TMP*scale,-TMP*scale,TMP*scale,-TMP*2.0*scale,TMP*2.0*scale);//glm::mat4(1.0);//glm::ortho<float>(0.0, SHADOW_W, 1.0, SHADOW_H);
      m_light[0] = m_lightProject * m_lightView;
      for(std::vector<std::shared_ptr<RenderableObjectExt>>::iterator it=meshs.begin(); it!=meshs.end(); it++)
	{
	  (*it)->setMatrixView(&m_lightView);
	  (*it)->setMatrixProject(&m_lightProject);
	  (*it)->setEyeDir(&sun);
	  (*it)->setSunDir(&sun);
	  (*it)->setShadowTex(depthTexArray);
	  (*it)->setMatrixLight(m_light);
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
  for(std::vector<std::shared_ptr<RenderableObjectExt>>::iterator it=meshs.begin(); it!=meshs.end(); it++)
    {
      camera->use((*it));
      (*it)->setSunDir(&sun);
      (*it)->setShadowTex(depthTexArray);
      (*it)->setMatrixLight(m_light2);
      (*it)->render();
    }
}

