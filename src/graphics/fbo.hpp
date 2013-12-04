#pragma once
/** @file graphics/fbo.hpp Frame buffer wrapping (mainly for postp effects) */

#include "graphics/opengl.hpp"
#include "graphics/buffer.hpp"
#include "util.hpp"
#include "graphics/glsl.hpp"
#include "graphics/base.hpp"

namespace graphics
{
#define N_SHADOW_BUFFERS 4
  /** A OpenGL frame buffer wrapper
   * This will implement a render-to-texture effect */
  class GLFrameBuffer
  {
  private:

    /** The last used fbo */
    GLint oldFBO;
    
  public:

    /** The framebuffer width */
    GLint fw;
    /** The framebuffer height */
    GLint fh;

#define MAX_PASSES 16
    /** The OpenGL id of this fbo */
    GLuint id_fbo;

    /** The optional OpenGL id of the depth rbo */
    GLuint id_depth;

    GLenum id_passes[MAX_PASSES];
    int numPasses;

    /** The optional OpenGL id of the depth texture */
    GLuint id_tex_depth;

    /** The optional OpenGL depth uniform */
    std::shared_ptr<GLSLUniform> unif_depth;

    /** Whether there is a depth buffer */
    bool hasDepth;

    /** Initialize framebuffer
     * @param w framebuffer width
     * @param h framebuffer height
     * @param glsl the shader to use */
    GLFrameBuffer(const int w, const int h, const bool useDepth=false, const std::string& unif_name="");

    /** Destroy framebuffer */
    ~GLFrameBuffer();

    /** Finalize passes */
    void update();

    /** Set this buffer as active */
    void use();

    /** Set this buffer's texture as active */
    void useTex();

    /** Draw the buffer
     * If you are chain-buffering, activate the next buffer before this, otherwise call unuse  */
    void draw();
    
    /** Switch back to default fbo */
    void unuse();
  };

  /** A color pass */
  class GLPass
  {
  private:
  public:

    /** The texture id */
    GLuint id_tex;

    /** The uniform for this texture */
    std::shared_ptr<graphics::GLSLUniform> unif;

    /** Initialize from a framebuffer
     * @param passNum the pass number */
    GLPass(std::shared_ptr<GLFrameBuffer> fbo, GLint passNum, const std::string& unif_name, const GLenum type=GL_RGBA);

    /** Destroy */
    ~GLPass();

    void use();

    /** Set this texture as active */
    void useTex();
  };

  /** A depth buffer FORMATTED pass */
  class GLDepthPass
  {
  private:
  public:

    /** The texture id */
    GLuint id_tex;

    /** The uniform for this texture */
    std::shared_ptr<graphics::GLSLUniform> unif;

    /** Initialize from a framebuffer
     * @param passNum the pass number */
    GLDepthPass(std::shared_ptr<GLFrameBuffer> fbo, GLint passNum, const std::string& unif_name, const GLenum type=GL_DEPTH_COMPONENT32F);

    /** Destroy */
    ~GLDepthPass();

    void use();

    /** Set this texture as active */
    void useTex();
  };
};
