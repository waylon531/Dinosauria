#include "game/rock.hpp"

std::shared_ptr<graphics::Material> material;
std::shared_ptr<graphics::Mesh> mesh;

void init_rocks()
{
  material = std::shared_ptr<graphics::Material>(new graphics::Material("res/materials/rock.mat"));
  mesh = MKPTR(graphics::Mesh, "res/models/rock.mesh", material, true);
}

Rock::Rock(std::shared_ptr<Landscape> ground)
{
  float x = rand()/float(RAND_MAX);
  x *= 500;
  x -= 250;
  float z = rand()/float(RAND_MAX);
  z *= 500;
  z -= 250;
  float y = ground->eval(x, z) + 2;
  pos = glm::vec3(x,y,z);
  body = std::shared_ptr<physics::RigidBody>(new physics::ConvexHull(mesh, pos, .1));
  m_model = glm::translate(pos);
}

void Rock::render()
{
  pos = body->getPosition();
  glm::vec3 rot = rad2deg(body->getRotation());
  //std::cout << pos.x << "," << pos.y << "," << pos.z << std::endl;
  m_model = glm::translate(pos) * glm::rotate(rot.z,glm::vec3(0,0,1)) * glm::rotate(rot.y,glm::vec3(0,1,0)) * glm::rotate(rot.x,glm::vec3(1,0,0));
  mesh->m_model = m_model;
  mesh->render();
}


void Rock::setMatrixView(glm::mat4* m)
{
  mesh->setMatrixView(m);
}

void Rock::setMatrixProject(glm::mat4* m)
{
  mesh->setMatrixProject(m);
}

void Rock::setWaterFlag(int* flag)
{
  mesh->setWaterFlag(flag);
}
