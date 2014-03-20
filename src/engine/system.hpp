//file: system.hpp
//purpose: abstract ECS System

//STL includes
#include <vector>
//custom includes
#include "engine/entity.hpp"

namespace engine
{
  class World;

  /** An abstract system to manipulate components of entities */
  class System
  {
  public:

    /** The parent world of this system */
    World* parent;

    /** The mask of this system */
    EntityMask mask;

    /** The set of entities belonging to this system */
    std::vector<unsigned int> entities;

    /** Execute this system
     * @param pulse time since last cycle */
    virtual void run(const float pulse) =0;
  };
};
