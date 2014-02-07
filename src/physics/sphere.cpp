#include "physics/sphere.hpp"

physics::Sphere::Sphere(const float radius, const glm::vec3& pos, const float mass)
{
  shape = new btSphereShape(radius);
  initialize(pos, mass);
}
