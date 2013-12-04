#pragma once
/** @file graphics/texture.hpp OpenGL texture wrapping and image loaders */

#include "util.hpp"
#include "graphics/opengl.hpp"
#include "graphics/types.hpp"
#include "graphics/glsl.hpp"

#include <png++/png.hpp>

namespace graphics
{

  /** An OpenGL abstract texture */
  class GLTexture
  {
  public:
    
    /** The GLSLUniform of to be used for shaders */
    std::shared_ptr<GLSLUniform> unif;

    virtual void use() =0;
  };
  
  /** An OpenGL 2D texture */
  class GLTexture2D: public GLTexture
  {
  private:
    
    /** The OpenGL texture id */
    GLuint id;
    
  public:

    /** The OpenGL texture unit */
    GLuint texUnit;
	
    /** Initialize from texture data
     * @param data texture data
     * @param w width
     * @param h height
     * @param unifName name of uniform variable */
    GLTexture2D(void* data, const int w, const int h, std::string unifName="", GLuint type=GL_RGBA, GLuint type2=GL_RGBA, GLuint type3=GL_FLOAT);

    /** Initialize from a png file
     * @param fname filename
     * @param unifName uniform name */
    GLTexture2D(const std::string& fname, const std::string& unifName="");
    
    /** Destroy */
    ~GLTexture2D();
    
    /** Setup so that this texture can be used for rendering */
    void use();
  };

  /** An OpenGL 3D texture */
  class GLTexture3D: public GLTexture
  {
  private:
    
    /** The OpenGL texture id */
    GLuint id;
    
  public:

    /** The OpenGL texture unit */
    GLuint texUnit;
	
    /** Initialize from texture data
     * @param data texture data
     * @param w width
     * @param h height
     * @param d depth
     * @param unifName name of uniform variable */
    GLTexture3D(void* data, const int w, const int h, const int d, std::string unifName="", GLuint type=GL_RGBA, GLuint type2=GL_RGBA, GLuint type3=GL_FLOAT);
    
    /** Destroy */
    ~GLTexture3D();
    
    /** Setup so that this texture can be used for rendering */
    void use();
  };
};
