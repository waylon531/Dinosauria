//file: util.hpp
//purpose: general utility functions

#pragma once

//STL includes
#include <string>
#include <iostream>
#include <map>
//std includes
#include <cstdlib>
//custom includes
#include "config.hpp"

/** Preform linear interpolation between two values
 * @param a first value
 * @param b second value
 * @param alpha mix factor
 * @return (1.0-alpha)*a+alpha*b */
template <typename T> T lerp(const T& a, const T& b, const float alpha)
{
  return (1.f-alpha)*a + alpha*b;
}
