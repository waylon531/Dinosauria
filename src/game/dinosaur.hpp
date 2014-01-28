#pragma once
/** @file game/dinosaur.hpp */

#include "graphics/mesh.hpp"
#include "graphics/bmesh.hpp"
#include <pugixml.hpp>
#include "game/landscape.hpp"

/** A dinosaur structure */
class Dinosaur: public graphics::RenderableObject
{
public:
  /** The dinosaur's mesh */
  std::shared_ptr<graphics::SkeletalMesh> mesh;

  /** The dinosaur's material */
  std::shared_ptr<graphics::Material> material;

  /** The description of the dinosaur */
  std::string description;

  /** The name of the dinosaur (capitilized) */
  std::string name;

  /** The movement speed of the dinosaur */
  float speed;

  /** Initialize from an XML file
   * @param fname file name */
  Dinosaur(const std::string& fname);

  ~Dinosaur();

  void render();
};

/** A list of all the dinosaurs */
extern std::vector<std::shared_ptr<Dinosaur>> dinosaurs;

/** Get a dinosaur by it's name
 * @param name name of the dinosaur to fetch */
extern std::shared_ptr<Dinosaur> getDinosaur(const std::string& name);

/** A single instance of a dinosaur in the game */
class DinosaurInstance : public graphics::RenderableObjectExt
{
public:

  /** The parent dinosaur template */
  std::shared_ptr<Dinosaur> parent;

  /** The dinosaur's location */
  glm::vec3 pos;

  /** The z rotation of the dinosaur */
  float zrot;

  /** The model matrix */
  glm::mat4 matrix;

  /** The current clock */
  float time;

  /** Initialize from a parent
   * @param dino parent dinosaur template */
  DinosaurInstance(std::shared_ptr<Dinosaur> dino);

  /** Initialize from a saved node in a file
   * @param node XML node */
  DinosaurInstance(pugi::xml_node& node);

  ~DinosaurInstance();

  /** Update the matrix */
  void update(std::shared_ptr<Landscape> ground);

  /** Save to an XML node */
  void save(pugi::xml_node& node);

  void setMatrixView(glm::mat4* view);
  void setMatrixProject(glm::mat4* project);
  void setWaterFlag(int* wflag)
  {
    parent->mesh->setWaterFlag(wflag);
  }

  void render();
};
