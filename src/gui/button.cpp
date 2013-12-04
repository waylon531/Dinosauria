#include "gui/button.hpp"

#define COLOR_DEFAULT glm::vec4(.75,.9,.75,.5)
#define COLOR_HOVER glm::vec4(.8,.9,1.,.5)
#define COLOR_DOWN glm::vec4(.5,.5,.5,.5)

gui::Button::Button(const std::string& txt, const glm::vec2& pos, Callback clb, std::shared_ptr<graphics::GLSLUniform> uniform) : Widget(pos), callback(clb)
{
#define XBORDER 10.0f
#define YBORDER 5.0f
  text = std::unique_ptr<StaticText>(new StaticText(txt, glm::vec4(0.0f,0.0f,0.0f,1.0f), position-glm::vec2(XBORDER,YBORDER), font_50pt));
  bbox = graphics::BBox2D(text->bbox.low-glm::vec2(XBORDER,2*YBORDER), text->bbox.high+glm::vec2(XBORDER,0*YBORDER));
  verts = new glm::vec2[4]{glm::vec2(bbox.low.x, bbox.low.y),
			   glm::vec2(bbox.low.x, bbox.high.y),
			   glm::vec2(bbox.high.x, bbox.high.y),
			   glm::vec2(bbox.high.x, bbox.low.y)};
  inds = new GLuint[6]{0,1,2,
		       0,2,3};
  graphics::GLSLAttributeSet attrs(std::vector<graphics::GLSLAttribute>({graphics::GLSLAttribute("position",2)}));
  b_verts = std::unique_ptr<graphics::GLVertexBuffer>(new graphics::GLVertexBuffer((void*)verts, 4, attrs));
  b_inds = std::unique_ptr<graphics::GLIndexBuffer>(new graphics::GLIndexBuffer(inds, 6));

  shader = std::unique_ptr<graphics::GLSL>(new graphics::GLSL(0));
  shader->addShader("res/shaders/button.vert", GL_VERTEX_SHADER);
  shader->addShader("res/shaders/button.frag", GL_FRAGMENT_SHADER);
  shader->linkShader();
  color = COLOR_DEFAULT;
  u_color = std::shared_ptr<graphics::GLSLUniform>(new graphics::GLSLUniform("color",&color));
  shader->attachUniform(u_color);
  shader->attachUniform(uniform);
}

gui::Button::~Button()
{
  text.reset();
  b_verts.reset();
  b_inds.reset();
  shader.reset();
  u_color.reset();
  delete[] verts;
  delete[] inds;
}

void gui::Button::render()
{
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  shader->use();
  b_verts->use();
  b_inds->use();
  b_inds->draw(GL_TRIANGLES);
  b_inds->unuse();
  b_verts->unuse();
  text->render();
}

void gui::Button::onMouseOver(const glm::vec2& pos)
{
  color = COLOR_HOVER;
}

void gui::Button::onNotMouseOver()
{
  color = COLOR_DEFAULT;
}

void gui::Button::onMouseClick(const glm::vec2& pos)
{
  color = COLOR_DOWN;
}
void gui::Button::onMouseRelease(const glm::vec2& pos)
{
  if(e_mouseOver)
    {
      onMouseOver(pos);
    }
  else
    {
      onNotMouseOver();
    }
  callback.call();
}
