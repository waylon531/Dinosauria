//A converter for the custom mesh format

#include "graphics/opengl.hpp"

#include "iostream"
#include "cstdlib"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>

struct attrib
{
  glm::vec3 pos;
  glm::vec3 normal;
  glm::vec2 texCoord;
  glm::vec3 tangent;
};

int main(int argc, char** argv)
{
  int nt,nv;
  nt=nv=0;
  attrib* verts;
  GLuint* inds;
  Assimp::Importer importer;
#define NORMAL_TYPE aiProcess_GenSmoothNormals
  const aiScene* pScene = importer.ReadFile(argv[1], aiProcess_Triangulate | NORMAL_TYPE);
  pScene = importer.ApplyPostProcessing(aiProcess_CalcTangentSpace);
  if(pScene)
    {
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
	      const aiVector3D* tangentptr = &(mesh->mTangents[i]);
	      aiVector3D tangent;
	      if(!mesh->mTangents)
		{
		  tangent = aiVector3D(0,0,0);
		}
	      else
		{
		  tangent = *tangentptr;
		}
	      attrib v = (attrib){glm::vec3(pos->x, pos->y, pos->z),
				  glm::vec3(normal->x, normal->y, normal->z),
				  glm::vec2(texCoord->x, texCoord->y),
				  glm::vec3(tangent.x, tangent.y, tangent.z)};
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
    }
  else
    {
      std::cout << "Error parsing '" << argv[1] << "': '" << importer.GetErrorString() << std::endl;
      return -1;
    }

  std::cout << "Triangles: " << nt << " Vertices: " << nv << std::endl;
  //write file
  FILE* f = fopen(argv[2],"w");
  int header[2] = {nv,nt};
  fwrite(header, sizeof(int), 2, f);
  fwrite(verts, sizeof(attrib), nv, f);
  fwrite(inds, sizeof(GLuint), nt*3, f);
  fclose(f);
  std::cout << "Done" << std::endl;
  
  return 0;
}
