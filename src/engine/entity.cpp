#include "engine/entity.hpp"

std::map<std::string, unsigned int> engine::entitySigTable;
unsigned int engine::currentEntitySig = 0;
unsigned int currentEntityID = 0;

engine::Entity::Entity(const unsigned int s, const std::string& n)
{
  sig  = s;
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

engine::Component engine::Entity::getComponent(const unsigned int id)
{
  for(int i=0; i<components.size(); i++)
    {
      if(components[i].ID == id) return components[i];
    }
  std::cout << "Error: could not find component with id " << id << " in this entity." << std::endl;
  raise(SIGTERM);
  return Component();
}
