/** @file the base class for systems to modify the entities */
#pragma once

#include "entity.hpp"

namespace engine
{

  /** A unit to preform actual computation on entities */
  class System
  {
  public:

    /** The signature used to determine which entities belong to this system */
    unsigned int filter;

    /** Preform the computation on a set of entities
     * @param entities list of entities used for this system */
    virtual void runSystem(std::vector<Entity>& entities) =0;
  };
};
