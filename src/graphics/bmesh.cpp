#include "graphics/bmesh.hpp"
#include <glm/gtx/quaternion.hpp>

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
	  return Bone(glm::mat4());
	}
      };
      boneInfo* bonestmp = new boneInfo[nBones];
      fread(bonestmp, sizeof(boneInfo), nBones, f);
      for(int i=0; i<nBones; i++)
	{
	  bones.push_back(bonestmp[i]);
	}
      int nNodes;
      fread(&nNodes, sizeof(int), 1, f);
      struct nodeInfo
      {
	int id;
	int nChildren;
	int nTranslationKeys;
	int nRotationKeys;
      };
      for(int i=0; i<nNodes; i++)
	{
	  nodeInfo node;
	  fread(&node, sizeof(nodeInfo), 1, f);
	  std::vector<int> children;
	  children.resize(node.nChildren);
	  fread(&children[0], sizeof(int), node.nChildren, f);
	  std::vector<TranslationKeyframe> tkeys;
	  tkeys.resize(node.nTranslationKeys);
	  fread(&tkeys[0], sizeof(TranslationKeyframe), node.nTranslationKeys, f);
	  std::vector<RotationKeyframe> rkeys;
	  rkeys.resize(node.nRotationKeys);
	  fread(&rkeys[0], sizeof(RotationKeyframe), node.nRotationKeys, f);
	  anim.nodes.push_back(ActionNode(node.id, children, rkeys, tkeys));
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

  for(int i=0; i<nBones; i++)
    {
      std::shared_ptr<GLSLUniform> unif(new GLSLUniform("boneTransforms["+asString(i)+"]", &bones[i].offset));
      material->addUniform(unif);
      shader_shadow->attachUniform(unif);
    }
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

void graphics::SkeletalMesh::setTime(float t)
{
  glm::mat4 tt;
  readNodes(anim.nodes[anim.nodes.size()-1], tt, t);
}

void graphics::SkeletalMesh::readNodes(const ActionNode& node, const glm::mat4& ptrans, const float time)
{
  glm::mat4 mnew = glm::mat4();
  if(node.boneId != -1)
    {
      mnew = interpMatrix(time, node);
    }
  bones[node.boneId].offset = /*ptrans */ mnew;
  for(int i=0; i<node.children.size(); i++)
    {
      readNodes(anim.nodes[node.children[i]], ptrans * mnew, time);
    }
}

glm::mat4 graphics::SkeletalMesh::interpMatrix(const float time, const ActionNode& node) const
{
  glm::vec3 translate = interpTranslation(time, node);
  glm::quat rotate = interpRotation(time, node);
  return glm::translate(translate)*glm::toMat4(rotate);
}

glm::vec3 graphics::SkeletalMesh::interpTranslation(const float time, const ActionNode& node) const
{
  if(node.lockeys.size() == 0) return glm::vec3(0.f,0.f,0.f);
  if(node.lockeys.size() < 2) return node.lockeys[0].value;
  int k0;
  for(k0=0; k0<node.lockeys.size(); k0++)
    {
      if(node.lockeys[k0].time > time)
	{
	  k0--;
	  break;
	}
    }
  //if(k0 == node.lockeys.size()-1) return node.lockeys[k0].value;
  int k1 = (k0+1)%node.rotkeys.size();
  float timeOffset = time - node.lockeys[k0].time;
  return glm::mix(node.lockeys[k0].value, node.lockeys[k1].value, timeOffset/(abs(node.rotkeys[k1].time-node.rotkeys[k0].time)));
}

glm::quat graphics::SkeletalMesh::interpRotation(const float time, const ActionNode& node) const
{
  if(node.rotkeys.size() == 0) return glm::quat();
  if(node.rotkeys.size() < 2) return node.rotkeys[0].value;
  int k0;
  for(k0=0; k0<node.rotkeys.size(); k0++)
    {
      if(node.rotkeys[k0].time > time)
	{
	  k0--;
	  break;
	}
    }
  //if(k0 == node.rotkeys.size()-1) return node.rotkeys[k0].value;
  int k1 = (k0+1)%node.rotkeys.size();
  float timeOffset = time - node.rotkeys[k0].time;
  return glm::fastMix(node.rotkeys[k0].value,node.rotkeys[k1].value, timeOffset/(abs(node.rotkeys[k1].time-node.rotkeys[k0].time)));
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
