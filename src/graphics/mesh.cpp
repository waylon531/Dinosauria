#include "graphics/mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>

graphics::Mesh::Mesh(const std::string& fname, std::shared_ptr<Material> m)
{
  material = m;
  //import
  Assimp::Importer importer;
#define NORMAL_TYPE aiProcess_GenSmoothNormals
  const aiScene* pScene = importer.ReadFile(fname.c_str(), aiProcess_Triangulate | NORMAL_TYPE);
  if(pScene)
    {
      int nv=0;
      int nt=0;
      for(unsigned int i=0; i<pScene->mNumMeshes; i++)
	{
	  nv += pScene->mMeshes[i]->mNumVertices;
	  nt += pScene->mMeshes[i]->mNumFaces;
	}
      verts = new attrib[nv];
      inds = new GLuint[nt*3];
      for(unsigned int mi=0; mi<pScene->mNumMeshes; mi++)
	{
	  const aiMesh* mesh = pScene->mMeshes[mi];
	  const aiVector3D Zero3D(0.0f,0.0f,0.0f);
	  for(unsigned int i=0; i<mesh->mNumVertices; i++)
	    {
	      const aiVector3D* pos = &(mesh->mVertices[i]);
	      const aiVector3D* normal = &(mesh->mNormals[i]);
	      const aiVector3D* texCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero3D;
	      attrib v = (attrib){glm::vec3(pos->x, pos->y, pos->z),
				  glm::vec3(normal->x, normal->y, normal->z),
				  glm::vec2(texCoord->x, texCoord->y)};
	      verts[i] = v;
	    }
	  for(unsigned int i=0; i<mesh->mNumFaces; i++)
	    {
	      const aiFace& f = mesh->mFaces[i];
	      inds[i*3+0] = f.mIndices[0];
	      inds[i*3+1] = f.mIndices[1];
	      inds[i*3+2] = f.mIndices[2];
	    }
	}
      GLSLAttributeSet attrs(std::vector<GLSLAttribute>({GLSLAttribute("position",3), GLSLAttribute("normal",3), GLSLAttribute("texCoord",2)}));
      b_vert = std::unique_ptr<GLVertexBuffer>(new GLVertexBuffer(verts, nv, attrs));
      b_ind = std::unique_ptr<GLIndexBuffer>(new GLIndexBuffer(inds, nt*3));
    }
  else
    {
      std::cout << "Error parsing '" << fname.c_str() << "': '" << importer.GetErrorString() << std::endl;
    }
  m_model = glm::mat4();
  initShadow();
}

graphics::Mesh::Mesh(attrib* v, GLuint* i, const int nv, const int ni, std::shared_ptr<Material> m)
{
  material = m;
  verts = v;
  inds = i;
  GLSLAttributeSet attrs(std::vector<GLSLAttribute>({GLSLAttribute("position",3), GLSLAttribute("normal",3), GLSLAttribute("texCoord",2)}));
  b_vert = std::unique_ptr<GLVertexBuffer>(new GLVertexBuffer(verts, nv, attrs));
  b_ind = std::unique_ptr<GLIndexBuffer>(new GLIndexBuffer(inds, ni*3));
  initShadow();
}

void graphics::Mesh::initShadow()
{
  shader_shadow = std::unique_ptr<GLSL>(new GLSL(0));
  shader_shadow->addShader("res/shaders/shadow.vert",GL_VERTEX_SHADER);
  shader_shadow->addShader("res/shaders/shadow.frag",GL_FRAGMENT_SHADER);
  shader_shadow->linkShader();
  u_light = std::shared_ptr<GLSLUniform>(new GLSLUniform("m_light",(glm::mat4*)NULL));
  shader_shadow->attachUniform(u_light);
}

graphics::Mesh::~Mesh()
{
  b_vert.reset();
  b_ind.reset();
  material.reset();
  delete[] verts;
  delete[] inds;
}

void graphics::Mesh::renderShadowPass()
{
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  shader_shadow->use();
  b_vert->use();
  b_ind->use();
  b_ind->draw(GL_TRIANGLES);
  b_vert->unuse();
  b_ind->unuse();
}

void graphics::Mesh::render()
{
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  material->setMatrixModel(&m_model);
  material->use();
  b_vert->use();
  b_ind->use();
  b_ind->draw(GL_TRIANGLES);
  b_vert->unuse();
  b_ind->unuse();
}

void graphics::Mesh::setMatrixView(glm::mat4* view)
{
  material->setMatrixView(view);
}

void graphics::Mesh::setMatrixProject(glm::mat4* project)
{
  material->setMatrixProject(project);
}

void graphics::Mesh::setWaterFlag(int* flag)
{
  material->setWaterFlag(flag);
}
