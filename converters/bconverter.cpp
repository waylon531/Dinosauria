//A converter for the custom mesh format with skinning support

#include "graphics/opengl.hpp"

#include "iostream"
#include "cstdlib"
#include <vector>
#include <string>
#include <map>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>

struct attrib
{
  glm::vec3 pos;
  glm::vec3 normal;
  glm::vec2 texCoord;
  int boneId[4];
  float boneWeight[4];

  void addBone(int id, float w)
  {
    for(int i=0; i<4; i++)
      {
        if(boneWeight[i] == 0.f)
	  {
	    boneWeight[i] = w;
	    boneId[i] = id;
	    return;
	  }
      }
    assert(false);
  }
};

struct Bone
{
  glm::mat4 offsetMatrix;
};

struct RotationKeyframe
{
  glm::quat value;
  float time;
  RotationKeyframe(const aiQuatKey& key)
  {
    time = key.mTime;
    memcpy(&value, &key.mValue, sizeof(glm::quat));
  }
  RotationKeyframe()
  {
  }
};

struct TranslationKeyframe
{
  glm::vec3 value;
  float time;
  TranslationKeyframe(const aiVectorKey& key)
  {
    time = key.mTime;
    memcpy(&value, &key.mValue, sizeof(glm::vec3));
  }
  TranslationKeyframe()
  {
  }
};


struct ActionNode
{
  int boneId;
  std::vector<RotationKeyframe> rotkeys;
  std::vector<TranslationKeyframe> lockeys;
  std::vector<int> children;
  ActionNode()
  {
  }
  ActionNode(aiNode* node, const aiAnimation* anim, std::map<std::string,int>& mapping, std::vector<ActionNode>* nodes)
  {
    if(mapping.find(std::string(node->mName.data)) == mapping.end())
      {
	boneId = -1;
      }
    else
      {
	boneId = mapping[std::string(node->mName.data)];
      }
    aiNodeAnim* anode=NULL;
    for(int i=0; i<anim->mNumChannels; i++)
      {
	if(std::string(anim->mChannels[i]->mNodeName.data) == std::string(node->mName.data))
	  {
	    anode = anim->mChannels[i];
	    break;
	  }
      }
    if(anode!=NULL)
      {
	for(int i=0; i<anode->mNumRotationKeys; i++)
	  {
	    rotkeys.push_back(RotationKeyframe(anode->mRotationKeys[i]));
	  }
	for(int i=0; i<anode->mNumPositionKeys; i++)
	  {
	    lockeys.push_back(TranslationKeyframe(anode->mPositionKeys[i]));
	  }
      }
    for(int i=0; i<node->mNumChildren; i++)
      {
	ActionNode node2 = ActionNode(node->mChildren[i], anim, mapping, nodes);
	children.push_back(nodes->size());
	nodes->push_back(node2);
      }
  }
};
struct Action
{
  std::vector<ActionNode> nodes;
  Action()
  {
  }
  Action(const aiScene* scene, std::map<std::string,int>& mapping)
  {
    nodes.push_back(ActionNode(scene->mRootNode, scene->mAnimations[0], mapping, &nodes));
  }
};

int main(int argc, char** argv)
{
  int nt,nv;
  nt=nv=0;
  attrib* verts;
  GLuint* inds;
  std::vector<Bone> skeletonInfo;
  std::map<std::string,int> skeletonMapping;
  int nBones = 0;
  Action anim;
  Assimp::Importer importer;
#define NORMAL_TYPE aiProcess_GenSmoothNormals | aiProcess_LimitBoneWeights
  const aiScene* pScene = importer.ReadFile(argv[1], aiProcess_Triangulate | NORMAL_TYPE);
  if(pScene)
    {
      for(unsigned int i=0; i<pScene->mNumMeshes; i++)
	{
	  nv += pScene->mMeshes[i]->mNumVertices;
	  nt += pScene->mMeshes[i]->mNumFaces;
	}
      verts = new attrib[nv];
      inds = new GLuint[nt*3];
      skeletonInfo.resize(nv);
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
	  //read bone data
	  for(int i=0; i<mesh->mNumBones; i++)
	    {
	      int boneIndex =0;
	      std::string boneName(mesh->mBones[i]->mName.data);
	      if(skeletonMapping.find(boneName) == skeletonMapping.end())
		{
		  boneIndex = nBones;
		  nBones++;
		  skeletonInfo.push_back(Bone());
		}
	      else
		{
		  boneIndex = skeletonMapping[boneName];
		}
	      memcpy(&skeletonInfo[boneIndex].offsetMatrix[0][0], &mesh->mBones[i]->mOffsetMatrix[0][0], sizeof(glm::mat4));
	      skeletonMapping[boneName] = boneIndex;
	      for(int j=0; j<mesh->mBones[i]->mNumWeights; j++)
		{
		  int vertexId = mesh->mBones[i]->mWeights[j].mVertexId;
		  float weight = mesh->mBones[i]->mWeights[j].mWeight;
		  verts[vertexId].addBone(boneIndex, weight);
		}
	    }
	  //read bone keyframes and animations
	  anim = Action(pScene, skeletonMapping);
	}
    }
  else
    {
      std::cout << "Error parsing '" << argv[1] << "': '" << importer.GetErrorString() << std::endl;
      return -1;
    }

  struct boneInfo
  {
    glm::mat4 offset;
    boneInfo(Bone b)
    {
      offset = b.offsetMatrix;
    }
  };
  std::vector<boneInfo> skeleton;
  for(std::map<std::string,int>::iterator it=skeletonMapping.begin(); it!=skeletonMapping.end(); it++)
    {
      skeleton.push_back(boneInfo(skeletonInfo[it->second]));
    }

  std::cout << "Triangles: " << nt << " Vertices: " << nv << " Bones: " << nBones << " Nodes: " << anim.nodes.size() << std::endl;
  //write file
  FILE* f = fopen(argv[2],"w");
  int header[3] = {nv,nt,nBones};
  fwrite(header, sizeof(int), 3, f);
  fwrite(verts, sizeof(attrib), nv, f);
  fwrite(inds, sizeof(GLuint), nt*3, f);
  fwrite(&skeleton[0], sizeof(boneInfo), nBones, f);

  //write animation
  int nNodes = anim.nodes.size();
  fwrite(&nNodes, sizeof(int), 1, f);
  for(int i=0; i<anim.nodes.size(); i++)
    {
      struct nodeInfo
      {
	int id;
	int nChildren;
	int nTranslationKeys;
	int nRotationKeys;
	nodeInfo(ActionNode node)
	{
	  id = node.boneId;
	  nChildren = node.children.size();
	  nTranslationKeys = node.lockeys.size();
	  nRotationKeys = node.rotkeys.size();
	}
      };
      nodeInfo node(anim.nodes[i]);
      fwrite(&node, sizeof(nodeInfo), 1, f);
      fwrite(&anim.nodes[i].children[0], sizeof(int), node.nChildren, f);
      for(int i2=0; i2<node.nTranslationKeys; i2++)
	{
	  fwrite(&anim.nodes[i].lockeys[i2], sizeof(TranslationKeyframe), 1, f);
	}
      for(int i2=0; i2<node.nRotationKeys; i2++)
	{
	  fwrite(&anim.nodes[i].rotkeys[i2], sizeof(RotationKeyframe), 1, f);
	}
    }
  fclose(f);
  std::cout << "Done" << std::endl;
  
  return 0;
}
