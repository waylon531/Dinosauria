#include "graphics/background.hpp"
#include <string>

graphics::Background::Background(const std::string& fname)
{
  texture = std::unique_ptr<GLTexture2D>(new GLTexture2D(fname,"tex_back"));
  verts = new glm::vec2[4]{glm::vec2(-1,-1),
			   glm::vec2( 1,-1),
			   glm::vec2(-1, 1),
			   glm::vec2( 1, 1)};
  GLSLAttributeSet attrs(std::vector<GLSLAttribute>({GLSLAttribute("position",2)}));
  b_verts = std::unique_ptr<GLVertexBuffer>(new GLVertexBuffer(verts, 4, attrs));
  shader = std::unique_ptr<GLSL>(new GLSL(0));
  shader->addShader("res/shaders/background.vert", GL_VERTEX_SHADER);
  shader->addShader("res/shaders/background.frag", GL_FRAGMENT_SHADER);
  shader->linkShader();

  texture->unif->setName("tex_back");
  shader->attachUniform(texture->unif);
}

graphics::Background::~Background()
{
  texture.reset();
  b_verts.reset();
  shader.reset();
  delete[] verts;
}

void graphics::Background::render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  b_verts->use();
  shader->use();
  texture->use();
  b_verts->draw(GL_TRIANGLE_STRIP);
  b_verts->unuse();
}
