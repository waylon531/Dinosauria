#pragma once

#include "engine/system.hpp"
#include "graphics/world.hpp"

/** A system to manage rendering objects */
class RenderSystem: public engine::System
{
public:
  
  std::shared_ptr<graphics::World> world;

  /** Initialize
   * @param w world to render to */
  RenderSystem(std::shared_ptr<graphics::World> w);

  /** Destroy */
  ~RenderSystem();

  void runSystem(std::vector<engine::Entity>& entities);
};
