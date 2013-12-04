#pragma once
/** @file pgen/water.hpp Water heightmap generation */

#include "util.hpp"

namespace pgen
{

  /** Generate a water heightmap
   * @param xres x resolution
   * @param yres y resolution
   * @param zres z resolution
   * @param hmap heightmap pointer */
  extern void genWater(const int xres, const int yres, const int zres, float* hmap);

  /** Load or generate a cached water texture
   * @param res resolution
   * @param tres temporal resoltuion
   * @param hmap heightmap pointer
   * @param nmap normal map pointer
   * @param tmap tangent map pointer
   * @param bmap bitangent map pointer */
  extern void cacheWater(const int res, const int tres, float* hmap, glm::vec3* nmap, glm::vec3* tmap, glm::vec3* bmap);
};
