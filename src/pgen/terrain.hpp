#pragma once
/** @file pgen/terrain.hpp Terrain generators */

#include "util.hpp"

namespace pgen
{

  /** Generate terrain
   * @param res terrain resolution
   * @param hmap heightmap pointer (allocated res*res)
   * @param bmap biome map pointer (allocated to res*res) */
  extern void generateTerrain(const int res, float* hmap, float* bmap, float* value, Callback& callback);
};
