#pragma once
/** @file pgen/fbm.hpp Fractal brownian motion generators */

#include "util.hpp"

namespace pgen
{

  /** Generate fBm
   * @param res resolution
   * @param octaves number of octaves to use
   * @param l lacunarity */
  extern float* gen_fBm(const int res, const int octaves, const float l, float* value, Callback& callback);
  
};
