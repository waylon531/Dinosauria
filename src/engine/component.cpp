#include "engine/component.hpp"

//STL includes
#include <map>

std::map<std::string, unsigned int> componentIDMap;
unsigned int currentComponentID = 0;

void engine::Component::load(const pugi::xml_node& node)
{
  throw std::runtime_error("XML loading not implemented for component"+getName());
}

void engine::Component::write(pugi::xml_node& node)
{
  throw std::runtime_error("XML storing not implemented for component"+getName());
}  

void engine::Component::registerComponent() const
{
  componentIDMap[getName()] = currentComponentID++;
}

unsigned int engine::Component::getID() const
{
  return componentIDMap[getName()];
}

unsigned int engine::getComponentID(const std::string& name)
{
  return componentIDMap[name];
}
