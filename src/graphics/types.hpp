#pragma once
/** @brief graphics/types.hpp Basic graphics types */

#include "graphics/opengl.hpp"

namespace graphics
{
  /** A 2D point */
  struct Point2D
  {
    /** X position */
    float x;

    /** Y position */
    float y;

    /** Construct from x and y
     * @param X x
     * @param Y y */
    Point2D(const float X, const float Y) : x(X), y(Y)
    {
    }

    Point2D(){}

    Point2D(const glm::vec2& v) : x(v.x), y(v.y)
    {
    }
};

  /** A RGB Color */
  struct Color
  {
    /** Red */
    float r;
    /** Green */
    float g;
    /** Blue */
    float b;
    /** Alpha */
    float a;

    /** Construct from r g and b
     * @param red
     * @param green
     * @param blue */
    Color(const float red, const float green, const float blue, const float alpha=1) : r(red), g(green), b(blue), a(alpha)
    {
    }

    Color(){}

    Color(const glm::vec4& v) : r(v.x), g(v.y), b(v.z), a(v.w)
    {
    }
  };

  /** A 2D bounding box */
  struct BBox2D
  {
    /** Lower bound */
    glm::vec2 low;

    /** Upper bound */
    glm::vec2 high;

    /** Construct from a low and a high
     * @param p0 low
     * @param p1 high */
    BBox2D(const glm::vec2& p0, const glm::vec2& p1) : low(p0), high(p1)
    {
    }

    /** Construct from a low and dimensions
     * @param p0 lower bound
     * @param w width
     * @param h height */
    BBox2D(const glm::vec2& p0, const float w, const float h) : low(p0), high(glm::vec2(p0.x+w, p0.y+h))
      {
      }

    BBox2D(){}

    /** Get the width
     * @return width */
    inline float getWidth() const
    {
      return high.x - low.x;
    }

    /** Get the height
     * @return height */
    inline float getHeight() const
    {
      return high.y - low.y;
    }

    /** Check if a point is inside this box
     * @param p point */
    bool inside(const glm::vec2& p) const
    {
      //std::cout << p.x << "," << low.x << "),(" << p.y << "," << low.y << std::endl;
      
      return (p.x>low.x) && (p.x<high.x) && (p.y>low.y) && (p.y<high.y);
    }
  };
};
