#include "graphics/fbo.hpp"

graphics::GLFrameBuffer::GLFrameBuffer(const int w, const int h, const bool useDepth, const std::string& unif_name) : hasDepth(useDepth), fw(w), fh(h)
{
  oldFBO = 0;
#undef DEPTH_COMPONENT
#define DEPTH_COMPONENT GL_DEPTH_COMPONENT32F
  glGenFramebuffers(1, &id_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, id_fbo);
  glEnable(GL_TEXTURE_2D);
  if(hasDepth)
    {
      glGenTextures(1, &id_tex_depth);
      glActiveTexture(GL_TEXTURE0+id_tex_depth);
      glBindTexture(GL_TEXTURE_2D, id_tex_depth);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_NONE);
      //glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
      glTexImage2D(GL_TEXTURE_2D, 0, DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id_tex_depth, 0);
      glBindTexture(GL_TEXTURE_2D,0);
    }
  else
    {
      glDrawBuffer(GL_NONE);
    }
  numPasses = 0;
  //glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  if(hasDepth)
    {
      unif_depth = (std::shared_ptr<GLSLUniform>(new GLSLUniform(unif_name.c_str(), (GLint*)&id_tex_depth)));
    }
}

void graphics::GLFrameBuffer::update()
{
  use();
  //glBindFramebuffer(GL_FRAMEBUFFER,id_fbo);
  glDrawBuffers(numPasses,id_passes);
  //glBindFramebuffer(GL_FRAMEBUFFER,0);
  unuse();
}
graphics::GLFrameBuffer::~GLFrameBuffer()
{
  glBindFramebuffer(GL_FRAMEBUFFER,0);
  glDeleteFramebuffers(1, &id_fbo);
  if(hasDepth)
    {
      //glDeleteRenderbuffers(1, &id_depth);
      glBindTexture(GL_TEXTURE_2D,0);
      glDeleteTextures(1, &id_tex_depth);
    }
  unif_depth.reset();
}

void graphics::GLFrameBuffer::use()
{
  glGetIntegerv(GL_FRAMEBUFFER_BINDING,&oldFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, id_fbo);
  if(hasDepth)
    {
      //glActiveTexture(GL_TEXTURE0+id_tex_depth);
      //glBindTexture(GL_TEXTURE_2D,0);
    }
  glDisable(GL_SCISSOR_TEST);
  glViewport(0,0,fw,fh);
}
void graphics::GLFrameBuffer::useTex()
{
  if(hasDepth)
    {
      glActiveTexture(GL_TEXTURE0+id_tex_depth);
      glBindTexture(GL_TEXTURE_2D,id_tex_depth);
    }
}

void graphics::GLFrameBuffer::unuse()
{
  glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
  current_viewport->use();
}

graphics::GLPass::GLPass(std::shared_ptr<GLFrameBuffer> fbo, GLint passNum, const std::string& unif_name, const GLenum type)
{
  glBindFramebuffer(GL_FRAMEBUFFER,fbo->id_fbo);
  glGenTextures(1, &id_tex);
  glActiveTexture(GL_TEXTURE0+id_tex);
  glBindTexture(GL_TEXTURE_2D, id_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, type, fbo->fw, fbo->fh, 0, GL_RGBA, GL_FLOAT, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+passNum, GL_TEXTURE_2D, id_tex, 0);
  fbo->id_passes[fbo->numPasses] = GL_COLOR_ATTACHMENT0+passNum;
  fbo->numPasses++;
  glBindTexture(GL_TEXTURE_2D,0);
  glBindFramebuffer(GL_FRAMEBUFFER,0);
  //use();
  unif = std::shared_ptr<graphics::GLSLUniform>(new graphics::GLSLUniform(unif_name.c_str(), (GLint*)&id_tex));
}

graphics::GLPass::~GLPass()
{
  glDeleteTextures(1,&id_tex);
}

void graphics::GLPass::use()
{
  //glActiveTexture(GL_TEXTURE0+id_tex);
  //glBindTexture(GL_TEXTURE_2D,0);
}

void graphics::GLPass::useTex()
{
  glActiveTexture(GL_TEXTURE0+id_tex);
  glBindTexture(GL_TEXTURE_2D,id_tex);
}


graphics::GLDepthPass::GLDepthPass(std::shared_ptr<GLFrameBuffer> fbo, GLint passNum, const std::string& unif_name, const GLenum type)
{
  glBindFramebuffer(GL_FRAMEBUFFER,fbo->id_fbo);
  glGenTextures(1, &id_tex);
  glActiveTexture(GL_TEXTURE0+id_tex);
  glBindTexture(GL_TEXTURE_2D, id_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, type, fbo->fw, fbo->fh, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id_tex, 0);
  fbo->id_passes[fbo->numPasses] = GL_DEPTH_ATTACHMENT;
  fbo->numPasses++;
  glBindTexture(GL_TEXTURE_2D,0);
  glBindFramebuffer(GL_FRAMEBUFFER,0);
  //use();
  unif = std::shared_ptr<graphics::GLSLUniform>(new graphics::GLSLUniform(unif_name.c_str(), (GLint*)&id_tex));
}

graphics::GLDepthPass::~GLDepthPass()
{
  glDeleteTextures(1,&id_tex);
}

void graphics::GLDepthPass::use()
{
  glActiveTexture(GL_TEXTURE0+id_tex);
  glBindTexture(GL_TEXTURE_2D,0);
}

void graphics::GLDepthPass::useTex()
{
  glActiveTexture(GL_TEXTURE0+id_tex);
  glBindTexture(GL_TEXTURE_2D,id_tex);
}

