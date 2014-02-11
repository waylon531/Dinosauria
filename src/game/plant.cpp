#include "game/plant.hpp"

#define NUM_MODELS 8

PlantSpecies::PlantSpecies(const std::string& fname)
{
  P3DInputStringStreamFile* file = new P3DInputStringStreamFile;
  file->Open(fname.c_str());
  plant = MKUPTR(P3DHLIPlantTemplate, file);
  file->Close();
  delete file;

  //generate models
  for(int i=0; i<NUM_MODELS; i++)
    {
      materials.push_back(MKPTR(graphics::Material, "res/materials/plant.mat"));
      models.push_back(MKPTR(PlantModel, *this, i));
    }
}

PlantSpecies::~PlantSpecies()
{
  plant.reset();
}

PlantModel::PlantModel(const PlantSpecies& species, const int seed)
{
  std::cout << "Creating plant model with seed " << seed << std::endl;
  P3DHLIPlantInstance* instance = species.plant->CreateInstance(seed+1);
  
  for(int i=0; i<3; i++)
    {
      int branchCount = instance->GetBranchCount(i);
      unsigned int* inds = new unsigned int[branchCount * species.plant->GetIndexCount(i, P3D_TRIANGLE_LIST)];
      for(int ii=0; ii<branchCount; ii++)
	{
	  species.plant->FillIndexBuffer(&(inds[species.plant->GetIndexCount(i,P3D_TRIANGLE_LIST)*ii]), i, P3D_TRIANGLE_LIST, P3D_UNSIGNED_INT, ii * species.plant->GetVAttrCountI(i));
	}
  
      graphics::Mesh::attrib* verts = new graphics::Mesh::attrib[instance->GetVAttrCountI(i)];
      P3DHLIVAttrBuffers buffer;
      buffer.AddAttr(P3D_ATTR_VERTEX, verts, 0, sizeof(graphics::Mesh::attrib));
      buffer.AddAttr(P3D_ATTR_NORMAL, verts, sizeof(glm::vec3), sizeof(graphics::Mesh::attrib));
      buffer.AddAttr(P3D_ATTR_TEXCOORD0, verts, 2*sizeof(glm::vec3), sizeof(graphics::Mesh::attrib));
      //buffer.AddAttr(P3D_ATTR_NORMAL, verts, 2*sizeof(glm::vec3)+sizeof(glm::vec2), sizeof(graphics::Mesh::attrib));
      instance->FillVAttrBuffersI(&buffer, i);

      graphics::Mesh* mesh = new graphics::Mesh(verts, inds, instance->GetVAttrCountI(i), branchCount * species.plant->GetIndexCount(i, P3D_TRIANGLE_LIST), species.materials[0]);
      meshes.push_back(std::shared_ptr<graphics::Mesh>(mesh));
    }
  
  delete instance;
}

PlantInstance::PlantInstance(std::shared_ptr<PlantSpecies> species, const int s, const glm::vec3& ppos, const float rot)
{
  parent = species;
  seed = s;
  pos = ppos;
  zrot = rot;
  m_model = glm::translate(pos) * glm::rotate(zrot, glm::vec3(0.f,1.f,0.f));
}
