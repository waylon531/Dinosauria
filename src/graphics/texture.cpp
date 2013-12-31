#include "graphics/texture.hpp"

graphics::GLTexture2D::GLTexture2D(void* data, const int w, const int h, std::string unifName, GLuint type, GLuint type2, GLuint type3)
{
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1,&id);
  texUnit = GL_TEXTURE0+id;
  use();
  glTexImage2D(GL_TEXTURE_2D, 0, type, w, h, 0, type2, type3, data);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  unif = std::shared_ptr<GLSLUniform>(new GLSLUniform(unifName.c_str(), (GLint*)&id));
}

graphics::GLTexture2D::GLTexture2D(const std::string& fname, const std::string& unifName)
{
  //load image
  png::image<png::rgba_pixel> image(fname);
  const int w = image.get_width();
  const int h = image.get_height();
  Color* data = new Color[w*h];
  for(int x=0; x<w; x++)
    {
      for(int y=0; y<h; y++)
	{
	  png::rgba_pixel& pixel = image[y][x];
	  data[x*h+y] = Color(pixel.red/256.0, pixel.green/256.0, pixel.blue/256.0, pixel.alpha/256.0);
	}
    }
  //load texture
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1,&id);
  texUnit = GL_TEXTURE0+id;
  //std::cout << id << "," << unifName << std::endl;
  use();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, h,w, 0, GL_RGBA, GL_FLOAT, data);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glHint(GL_GENERATE_MIPMAP_HINT, GL_FASTEST);
  glGenerateMipmap(GL_TEXTURE_2D);
  unif = std::shared_ptr<GLSLUniform>(new GLSLUniform(unifName.c_str(), (GLint*)&id));
}

graphics::GLTexture2D::~GLTexture2D()
{
  glDeleteTextures(1,&id);
  unif.reset();
}

void graphics::GLTexture2D::use()
{
  glEnable(GL_TEXTURE_2D);
  glActiveTexture(texUnit);
  glBindTexture(GL_TEXTURE_2D, id);
}


graphics::GLTexture3D::GLTexture3D(void* data, const int w, const int h, const int d, std::string unifName, GLuint type, GLuint type2, GLuint type3)
{
  glEnable(GL_TEXTURE_3D);
  glGenTextures(1,&id);
  texUnit = GL_TEXTURE0+id;
  use();
  glTexImage3D(GL_TEXTURE_3D, 0, type, w, h, d, 0, type2, type3, data);
  glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  unif = std::shared_ptr<GLSLUniform>(new GLSLUniform(unifName.c_str(), (GLint*)&id));
}

graphics::GLTexture3D::~GLTexture3D()
{
  glDeleteTextures(1,&id);
  unif.reset();
}

void graphics::GLTexture3D::use()
{
  glEnable(GL_TEXTURE_3D);
  glActiveTexture(texUnit);
  glBindTexture(GL_TEXTURE_3D, id);
}
