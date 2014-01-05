#include "graphics/bmesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>

graphics::SkeletalMesh::SkeletalMesh(const std::string& fname, std::shared_ptr<Material> m, bool isBin)
{
  int nv,nt,nBones;
  if(isBin)
    {
      //parse custom binary format
      FILE* f = fopen(fname.c_str(),"rb");
      int header[3];
      fread(header, sizeof(int), 3, f);
      nv = header[0];
      nt = header[1];
      nBones = header[2];
      verts = new attrib[nv];
      inds = new GLuint[nt*3];
      fread(verts, sizeof(attrib), nv, f);
      fread(inds, sizeof(GLuint), nt*3, f);
      //read the bones  
      struct boneInfo
      {
        glm::mat4 offset;
        operator Bone() const
	{
	  return Bone(offset);
	}
      };
      boneInfo* bonestmp = new boneInfo[nBones];
      fread(bonestmp, sizeof(boneInfo), nBones, f);
      for(int i=0; i<nBones; i++)
	{
	  bones.push_back(bonestmp[i]);
	}
      fclose(f);
    }
  else
    {
      std::cout << "Direct mesh parsing not implemented for skeletal meshes!" << std::endl;
    }
  material = m;
  GLSLAttributeSet attrs(std::vector<GLSLAttribute>({GLSLAttribute("position",3), GLSLAttribute("normal",3), GLSLAttribute("texCoord",2), GLSLAttribute("boneIds",4,GL_INT), GLSLAttribute("boneWeights",4)}));
  b_vert = std::unique_ptr<GLVertexBuffer>(new GLVertexBuffer(verts, nv, attrs));
  b_ind = std::unique_ptr<GLIndexBuffer>(new GLIndexBuffer(inds, nt*3));
  m_model = glm::mat4();
  initShadow();  
}

void graphics::SkeletalMesh::initShadow()
{
  shader_shadow = std::unique_ptr<GLSL>(new GLSL(0));
  shader_shadow->addShader("res/shaders/shadow_bones.vert",GL_VERTEX_SHADER);
  shader_shadow->addShader("res/shaders/shadow.frag",GL_FRAGMENT_SHADER);
  shader_shadow->linkShader();
  u_light = std::shared_ptr<GLSLUniform>(new GLSLUniform("m_light",(glm::mat4*)NULL));
  shader_shadow->attachUniform(u_light);
}

graphics::SkeletalMesh::~SkeletalMesh()
{
  b_vert.reset();
  b_ind.reset();
  material.reset();
  delete[] verts;
  delete[] inds;
}

void graphics::SkeletalMesh::renderShadowPass()
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

void graphics::SkeletalMesh::render()
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

void graphics::SkeletalMesh::setMatrixView(glm::mat4* view)
{
  material->setMatrixView(view);
}

void graphics::SkeletalMesh::setMatrixProject(glm::mat4* project)
{
  material->setMatrixProject(project);
}

void graphics::SkeletalMesh::setWaterFlag(int* flag)
{
  material->setWaterFlag(flag);
}
