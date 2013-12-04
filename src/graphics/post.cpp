#include "graphics/post.hpp"

graphics::Compositor::Compositor(std::shared_ptr<graphics::GLSL> glsl) : shader(glsl)
{
  //create vbo
  verts = new glm::vec2[4]{glm::vec2(-1,-1),
			   glm::vec2(-1, 1),
			   glm::vec2(1,-1),
			   glm::vec2(1,1)};
  GLSLAttributeSet attrs(std::vector<GLSLAttribute>({GLSLAttribute("position",2)}));
  b_vert = std::unique_ptr<GLVertexBuffer>(new GLVertexBuffer((void*)verts, 4, attrs));
}

graphics::Compositor::~Compositor()
{
  b_vert.reset();
  delete[] verts;
}

void graphics::Compositor::draw()
{
  //glUseProgram(0);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  b_vert->use();
  shader->use();
  b_vert->draw(GL_TRIANGLE_STRIP);
  b_vert->unuse();
}
