#pragma once
/** @file game/rock.hpp */

#include "graphics/material.hpp"
#include "graphics/mesh.hpp"
#include "physics/hull.hpp"
#include "game/landscape.hpp"
#include <pugixml.hpp>

/** Initialize rock data */
extern void init_rocks();

/** A rock */
class Rock: public graphics::RenderableObjectExt
{
public:
  
  /** The rigid body */
  std::shared_ptr<physics::RigidBody> body;

  /** The model matrix */
  glm::mat4 m_model;

  /** The rock's position */
  glm::vec3 pos;
  
  /** Initialize randomly */
  Rock(std::shared_ptr<Landscape> ground);

  /** Initialize from a xml node
   * @param node node to read from */
  Rock(pugi::xml_node& node);

  void setMatrixView(glm::mat4* m);
  void setMatrixProject(glm::mat4* m);
  void setWaterFlag(int* flag);
  void render();
};
