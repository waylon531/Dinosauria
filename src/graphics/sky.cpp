#include "graphics/sky.hpp"

graphics::Sky::Sky(const glm::vec3& c) : color(c)
{
}

void graphics::Sky::render()
{
  glClearColor(color.x, color.y, color.z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
