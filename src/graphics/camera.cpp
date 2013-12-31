#include "graphics/camera.hpp"

graphics::Camera::Camera(const glm::vec3& _pos, const glm::vec3& _look, const float _aspect) : pos(_pos), look(_look), aspect(_aspect)
{
  mat_view = glm::lookAt(pos,
			 look,
			 glm::vec3(0.f,1.f,0.f));
  mat_project = glm::perspective(90.0f,aspect,0.5f,200.0f);
}

void graphics::Camera::use(std::shared_ptr<RenderableObjectExt> material)
{
  material->setMatrixView(&mat_view);
  material->setMatrixProject(&mat_project);
}

void graphics::Camera::update()
{
  mat_view = glm::lookAt(pos,
			 look,
			 glm::vec3(0.f,1.f,0.f));
  dir = glm::normalize(look-pos);
}

void graphics::Camera::setLook(const glm::vec3& _look)
{
  look = _look;
  update();
}

void graphics::Camera::setPos(const glm::vec3& _pos)
{
  pos = _pos;
  update();
}
