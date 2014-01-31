#pragma once
/** @file util.hpp Utility functions that are usefull in general */

#include "graphics/opengl.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <csignal>
#include <memory>
#include <vector>

#define MKPTR(type,args...) std::shared_ptr<type>(new type(args))

using std::max;
using std::min;

/** A callback manager supporting member functions */
struct Callback
{
  /** The pointer to the object */
  void* _this;
  /** The pointer to the function */
  void (*ptr)(void*);
  Callback(void* This, void (*Ptr)(void*)) : _this(This), ptr(Ptr)
  {
  }
  inline void call()
  {
    ptr(_this);
  }
};

/** @brief Load entire ASCII file into string
 * @param file file object
 * @param str string object */
extern void loadFileToString(std::ifstream& file, std::string& str);

/** @brief Error interface
 * @param message error message */
extern void dError(const std::string& message);

/** @brief Warning interface
 * @param message warning message */
extern void dWarning(const std::string& message);

/** @brief Get a wchar_t* from an std::string
 * @param str string
 * @return wchar_t* version */
extern wchar_t* getWchar(const std::string& str);

/** Use a sstream to convert a value to a string
 * @param T type of value
 * @param x value
 * @return string representation */
template <typename T> std::string asString(const T x)
{
  std::stringstream ss;
  ss << x;
  return ss.str();
}

/** Clamp a value into a range
 * @param x value
 * @param a lower bound
 * @param b upper bound
 * @return clampped value */
template <typename T> inline T clamp(const T& x, const T& a, const T& b)
{
  if(x<a) return a;
  if(x>b) return b;
  return x;
}

/** Preform linear interpolation on a value
 * @param a first value
 * @param b second value
 * @param alpha alpha value
 * @return interpolated value */
template <typename T> inline T lerp(const T& a, const T& b, const float alpha)
{
  return (1.f-alpha)*a + alpha*b;
}

/** Preform cosine interpolation on a value
 * @param a first value
 * @param b second value
 * @param alpha alpha value
 * @return interpolated value */
template <typename T> inline T cerp(const T& a, const T& b, const float alpha)
{
float ft = alpha * M_PI;
float f = (1.f - cosf(ft)) * .5f;
  return f*a + (1.f-f)*b;
}

/** Preform 2D linear interpolation on a value
 * @param p00 lower left
 * @param p01 upper left
 * @param p11 upper right
 * @param p10 lower right
 * @param x x coordinate
 * @Param y y coordinate
 * @return interpolated value */
template <typename T> inline T lerp2D(const T& p00, const T& p01, const T& p10, const T& p11, const float x, const float y)
{
  return cerp(cerp(p00,p01,x),cerp(p10,p11,x),y);
}

/** Preform 2D linear interpolation to sample a matrix
 * Note that x and y are not normalized to xres and yres
 * @param a array to sample
 * @param xres x resolution
 * @param yres y resolution
 * @param x x position to sample
 * @param y y position to sample
 * @return interpolated value */
template <typename T> inline T lerpArray2D(T* a, const int xres, const int yres, const float x, const float y)
{
  int xx = (int)x;
  int yy = (int)y;
  float ox = x-xx;
  float oy = y-yy;
  const float s11 = a[xx*xres + yy];
  const float s01 = a[xx*xres + (yy+1)%yres];
  const float s10 = a[((xx+1)%xres)*xres + yy];
  const float s00 = a[((xx+1)%xres)*xres + (yy+1)%yres];
  return lerp2D(s00,s01,s10,s11, ox, oy);
}

/** Compute the normal of a hieghtmap triangle
 * @param p00
 * @param p01
 * @param p10
 * @return normal */
extern glm::vec3 triangleNormal(const float p00, const float p01, const float p10, const float s);

/** Compute the gradient of an array
 * @param a array
 * @param xres x resolution of array
 * @param yres y resolution of array
 * @return normal map */
extern void computeGradient(float* a, const int xres, const int yres, glm::vec3* tangents, glm::vec3* bitangents, glm::vec3* normals, float*,Callback&);

/** Compute the gradient of an array in 3D
 * @param a array
 * @param xres x resolution of array
 * @param yres y resolution of array
 * @param zres z resolution of array */
extern void computeGradient3D(float* a, const int xres, const int yres, const int zres, glm::vec3* tangents, glm::vec3* bitangents, glm::vec3* normals);

/** Parse a vec3 from a string */
inline glm::vec3 parseVec3(const std::string& str)
{
glm::vec3 tmp;
  std::stringstream ss(str);
  ss >> tmp.x >> tmp.y >> tmp.z;
  return tmp;
}

/** Write a 2D array of floats to a file
 * @param xres x resolution
 * @param yres y resolution
 * @param fname file name
 * @param a array */
extern void writeFloatFile(const int xres, const int yres, const std::string& fname, float* a);

/** Read a 2D array of floats from a file
 * @param xres x resolution
 * @param yres y resolution
 * @param fname file name
 * @return float array */
extern float* readFloatFile(const int xres, const int yres, const std::string& fname);

/** A general game exception */
class Exception
{
public:

  /** The error message */
  std::string message;

  /** The line that it was called from */
  int line;
  /** The file that it was called from */
  std::string file;

  /** Initialize
   * @param msg message
   * @param l line
   * @param f file */
  Exception(const std::string& msg, const int l, const std::string& f) : message(msg), line(l), file(f)
  {
  }

  /** Print output to console */
  void report() const
  {
    std::cerr << "Execption (" << file << ":" << line << "), \"" << message << "\"" << std::endl;
  }
};
