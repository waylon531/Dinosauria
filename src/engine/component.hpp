//file: component.hpp
//purpose: abstract ECS component

#pragma once

//STL includes
#include <memory>
//custom external includes
#include <pugixml.hpp>

namespace engine
{
  
  /** An abstract ECS component to hold _only_ data */
  class Component
  {
  public:

    /** Get the name of this type of component
     * @return component string identitifier */
    virtual std::string getName() const =0;

    /** Get the component ID of this component
     * @return component numeric identifier */
    unsigned int getID() const;

    /** Register this component type with an ID */
    void registerComponent() const;

    /** Parse this component from an XML node
     * @param node XML node to parse from */
    virtual void load(const pugi::xml_node& node);

    /** Write this component to an XML node
     * @param node XML node to write to */
    virtual void write(pugi::xml_node& node);
  };

  /** Get the numeric identifier of a component string name
   * @param name name of component
   * @return numeric ID of component with name name */
  extern unsigned int getComponentID(const std::string& name);
}
