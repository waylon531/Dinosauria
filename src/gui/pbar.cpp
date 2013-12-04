#include "gui/pbar.hpp"

#define BAR_WIDTH 75

gui::ProgressBar::ProgressBar(const glm::vec2& pos, const float len) : length(len), position(pos), value(0.f)
{
  verts_back = new glm::vec2[4]{glm::vec2(pos.x,pos.y),
				glm::vec2(pos.x+len,pos.y),
				glm::vec2(pos.x,pos.y+BAR_WIDTH),
				glm::vec2(pos.x+len,pos.y+BAR_WIDTH)};
  verts_front = new glm::vec2[4]{glm::vec2(pos.x,pos.y),
				 glm::vec2(pos.x,pos.y),
				 glm::vec2(pos.x,pos.y+BAR_WIDTH),
				 glm::vec2(pos.x,pos.y+BAR_WIDTH)};
  bbox = graphics::BBox2D(pos,len,BAR_WIDTH);
  graphics::GLSLAttributeSet attrs(std::vector<graphics::GLSLAttribute>({graphics::GLSLAttribute("position",2)}));
  b_verts_back = std::shared_ptr<graphics::GLVertexBuffer>(new graphics::GLVertexBuffer((void*)verts_back, 4, attrs));
  b_verts_front = std::shared_ptr<graphics::GLVertexBufferDynamic>(new graphics::GLVertexBufferDynamic(attrs));
  update();
  shader = std::shared_ptr<graphics::GLSL>(new graphics::GLSL(0));
  shader->addShader("res/shaders/bar.vert", GL_VERTEX_SHADER);
  shader->addShader("res/shaders/bar.frag", GL_FRAGMENT_SHADER);
  shader->linkShader();
  shader->attachUniform(graphics::u_ortho);
  u_color = std::shared_ptr<graphics::GLSLUniform>(new graphics::GLSLUniform("color",&color));
  shader->attachUniform(u_color);
}

gui::ProgressBar::~ProgressBar()
{
  b_verts_back.reset();
  b_verts_front.reset();
  shader.reset();
  u_color.reset();
  delete[] verts_back;
  delete[] verts_front;
}

void gui::ProgressBar::update()
{
  verts_front[1].x = position.x + length*value;
  verts_front[3].x = position.x + length*value;
  b_verts_front->update((void*)verts_front, 4);
}

void gui::ProgressBar::render()
{
  glDisable(GL_DEPTH_TEST);
  b_verts_back->use();
  color = glm::vec4(.5f,.0f,.5f,1.f);
  shader->use();
  b_verts_back->draw(GL_TRIANGLE_STRIP);
  b_verts_back->unuse();
  b_verts_front->use();
  color = glm::vec4(0.f,0.5f,1.f,1.f);
  shader->use();
  b_verts_front->draw(GL_TRIANGLE_STRIP);
  b_verts_front->unuse();
}
