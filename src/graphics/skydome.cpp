#include "graphics/skydome.hpp"

graphics::Skydome::Skydome()
{
  material = MKPTR(Material, "res/materials/skydome.mat");
  mesh = MKPTR(Mesh, "res/models/skydome.mesh", material, true);
}

void graphics::Skydome::render()
{
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  mesh->render();
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
}

void graphics::Skydome::renderShadowPass()
{
}
