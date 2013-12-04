#include "graphics/material.hpp"

#include <fstream>

graphics::Material::Material(const std::string& fname)
{
  shader = std::unique_ptr<GLSL>(new GLSL(0));
  std::ifstream f(fname);
  assert(f.good());
  std::string cword;
  std::string cword2;
  while(!f.eof())
    {
      f >> cword;
      if(cword == "vert")
	{
	  //vertex shader
	  f >> cword;
	  shader->addShader(cword, GL_VERTEX_SHADER);
	}
      else if(cword == "ctrl")
	{
	  //tesselation control shader
	  f >> cword;
	  shader->addShader(cword, GL_TESS_CONTROL_SHADER);
	}
      else if(cword == "eval")
	{
	  //tesselation evaluation shader
	  f >> cword;
	  shader->addShader(cword, GL_TESS_EVALUATION_SHADER);
	}
      else if(cword == "geom")
	{
	  //geometry shader
	  f >> cword;
	  shader->addShader(cword, GL_GEOMETRY_SHADER);
	}
      else if(cword == "frag")
	{
	  //fragment shader
	  f >> cword;
	  shader->addShader(cword, GL_FRAGMENT_SHADER);
	}
      else if(cword == "link")
	{
	  shader->linkShader();
	}
      else if(cword == "tex")
	{
	  //texture
	  f >> cword;
	  f >> cword2;
	  std::shared_ptr<GLTexture2D> tex(new GLTexture2D(cword,cword2));
	  addTexture(tex);
	  tex.reset();
	}
    }
  glm::mat4* m_model = NULL;
  glm::mat4* m_view = NULL;
  glm::mat4* m_project = NULL;
  glm::vec3* eye = NULL;
  glm::vec3* sun = NULL;
  GLint* shadow = NULL;
  u_model = std::shared_ptr<GLSLUniform>(new GLSLUniform("m_model", m_model));
  uniforms.push_back(u_model);
  shader->attachUniform(u_model);
  u_view = std::shared_ptr<GLSLUniform>(new GLSLUniform("m_view", m_view));
  uniforms.push_back(u_view);
  shader->attachUniform(u_view);
  u_project = std::shared_ptr<GLSLUniform>(new GLSLUniform("m_project", m_project));
  uniforms.push_back(u_project);
  shader->attachUniform(u_project);
  u_eye = std::shared_ptr<GLSLUniform>(new GLSLUniform("eyeDir", eye));
  uniforms.push_back(u_eye);
  shader->attachUniform(u_eye);
  u_sun = std::shared_ptr<GLSLUniform>(new GLSLUniform("sunDir", sun));
  uniforms.push_back(u_sun);
  shader->attachUniform(u_sun);
  for(int b=0; b<N_SHADOW_BUFFERS; b++)
    {
      u_shadow[b] = std::shared_ptr<GLSLUniform>(new GLSLUniform(("tex_shadow["+asString(b)+"]").c_str(), shadow));
      uniforms.push_back(u_shadow[b]);
      shader->attachUniform(u_shadow[b]);

      u_lightMatrix[b] = std::shared_ptr<GLSLUniform>(new GLSLUniform(("m_light["+asString(b)+"]").c_str(), (glm::mat4*)NULL));
      uniforms.push_back(u_lightMatrix[b]);
      shader->attachUniform(u_lightMatrix[b]);

    }
}

void graphics::Material::setMatrixModel(glm::mat4* m)
{
  u_model->value = (void*)m;
}
void graphics::Material::setMatrixView(glm::mat4* m)
{
  u_view->value = (void*)m;
}
void graphics::Material::setMatrixProject(glm::mat4* m)
{
  u_project->value = (void*)m;
}
void graphics::Material::setEyeDir(glm::vec3* dir)
{
  u_eye->value = (void*)dir;
}
void graphics::Material::setSunDir(glm::vec3* dir)
{
  u_sun->value = (void*)dir;
}
void graphics::Material::setShadowTex(GLint* tex)
{
  for(int b=0; b<N_SHADOW_BUFFERS; b++)
    {
      u_shadow[b]->value = (void*)&tex[b];
    }
}
void graphics::Material::setMatrixLight(glm::mat4* m)
{
  for(int b=0; b<N_SHADOW_BUFFERS; b++)
    {
      u_lightMatrix[b]->value = (void*)&m[b];
    }
}

void graphics::Material::addTexture(std::shared_ptr<GLTexture> tex)
{
  textures.push_back(tex);
  shader->attachUniform(tex->unif);
}

void graphics::Material::addUniform(std::shared_ptr<GLSLUniform> u)
{
  uniforms.push_back(u);
  shader->attachUniform(u);
}

graphics::Material::~Material()
{
  shader.reset();
  u_model.reset();
  u_view.reset();
  u_project.reset();
  //u_shadow.reset();
  //u_lightMatrix.reset();
}

void graphics::Material::use()
{
  glPatchParameteri(GL_PATCH_VERTICES, 3);
  for(std::vector<std::shared_ptr<GLTexture>>::iterator it=textures.begin(); it!=textures.end(); it++)
    {
      (*it)->use();
    }
  
  shader->use();
}
