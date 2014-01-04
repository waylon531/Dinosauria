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
  int* wflag = NULL;
  u_model = std::shared_ptr<GLSLUniform>(new GLSLUniform("m_model", m_model));
  uniforms.push_back(u_model);
  shader->attachUniform(u_model);
  u_view = std::shared_ptr<GLSLUniform>(new GLSLUniform("m_view", m_view));
  uniforms.push_back(u_view);
  shader->attachUniform(u_view);
  u_project = std::shared_ptr<GLSLUniform>(new GLSLUniform("m_project", m_project));
  uniforms.push_back(u_project);
  shader->attachUniform(u_project);
  u_wflag = MKPTR(GLSLUniform,"waterFlag", wflag);
  uniforms.push_back(u_wflag);
  shader->attachUniform(u_wflag);
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
void graphics::Material::setWaterFlag(int* flag)
{
  u_wflag->value = (void*)flag;
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
  u_wflag.reset();
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
