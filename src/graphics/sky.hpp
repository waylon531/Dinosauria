#pragma once
/** @file graphics/sky.hpp */

#include "graphics/opengl.hpp"
#include "graphics/rendObj.hpp"

namespace graphics
{

  /** A object to clear the background */
  class Sky: public RenderableObject
  {
  public:
    
    /** The sky color */
    glm::vec3 color;

    /** Initialize
     * @param c color of the sky */
    Sky(const glm::vec3& c);

    void render();
  };

};
