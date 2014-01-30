#include "engine/entity.hpp"

unsigned int currentEntityID = 0;

engine::Entity::Entity(const std::string& n)
{
  name = n;
  id = currentEntityID++;
}

engine::Entity::~Entity()
{
  for(int i=0; i<components.size(); i++)
    {
      components[i].destroyValue();
    }
}

void engine::Entity::addComponent(const Component& component)
{
  components.push_back(component);
}

engine::Component engine::Entity::getComponent(const std::string& name)
{
  unsigned int id = getComponentID(name);
  for(int i=0; i<components.size(); i++)
    {
      if(components[i].ID == id) return components[i];
    }
  std::cout << "Error: could not find component " << name << " with id " << id << " in this entity." << std::endl;
  raise(SIGTERM);
  return Component();
}
