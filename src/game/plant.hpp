#pragma once
/** @file game/plant.hpp Abstract definition of a plant */

#include "graphics/buffer.hpp"
#include "graphics/ibo.hpp"
#include "graphics/texture.hpp"
#include "graphics/mesh.hpp"
#include "ngpcore/p3dhli.h"
#include "ngpcore/p3diostream.h"
#include "ngpcore/p3dplant.h"

class PlantSpecies;

/** A 3D model for a plant */
class PlantModel
{
public:

  /** The list of branch group meshes */
  std::vector<std::shared_ptr<graphics::Mesh>> meshes;

  /** Initialize
   * @param species plant template to generate from
   * @param seed random seed to use */
  PlantModel(const PlantSpecies& species, const int seed);
};

/** A plant template to generate plant models for a single species */
class PlantSpecies
{
public:
  /** The HLI plant template storage */
  std::unique_ptr<P3DHLIPlantTemplate> plant;
  
  /** The list of models */
  std::vector<std::shared_ptr<PlantModel>> models;

  /** The list of branch group materials */
  std::vector<std::shared_ptr<graphics::Material>> materials;

  /** Initialize
   * @param fname filename to load plant from */
  PlantSpecies(const std::string& fname);

  /** Initialize */
  PlantSpecies()
  {
  }

  /** Destroy */
  ~PlantSpecies();

  /** Get a random instance
   * @return plant instance */
  //std::shared_ptr<PlantInstance> getInstance();
};

/** A single instance of a plant */
class PlantInstance: public graphics::RenderableObjectExt
{
public:

  /** The plant species */
  std::shared_ptr<PlantSpecies> parent;

  /** The random seed index */
  int seed;

  /** The location */
  glm::vec3 pos;

  /** The z axis rotation */
  float zrot;

  /** The model matrix */
  glm::mat4 m_model;

  /** Generate with a random position and rotation from a species
   * @param species species to use
   * @param seed random seed
   * @param pos position of plant
   * @param rot rotation */
  PlantInstance(std::shared_ptr<PlantSpecies> species, const int s, const glm::vec3& pos, const float rot);

  void render()
  {
    for(int i=0; i<parent->models[seed]->meshes.size(); i++)
      {
	parent->models[seed]->meshes[i]->m_model = m_model;
	parent->models[seed]->meshes[i]->render();
      }
  }

  void setMatrixView(glm::mat4* m)
  {
    for(int i=0; i<parent->models[seed]->meshes.size(); i++)
      {
	parent->models[seed]->meshes[i]->setMatrixView(m);
      }
  }
  void setMatrixProject(glm::mat4* m)
  {
    for(int i=0; i<parent->models[seed]->meshes.size(); i++)
      {
	parent->models[seed]->meshes[i]->setMatrixProject(m);
      }
  }
  void setWaterFlag(int* m)
  {
    for(int i=0; i<parent->models[seed]->meshes.size(); i++)
      {
	parent->models[seed]->meshes[i]->setWaterFlag(m);
      }
  }
};
