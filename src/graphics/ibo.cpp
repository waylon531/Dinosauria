#include "graphics/ibo.hpp"

namespace graphics
{
  //GLIndexBuffer methods

  GLIndexBuffer::GLIndexBuffer(GLuint* Data, const int NumVerts)
  {
    data = Data;
    glGenBuffers(1,&id);
    numVerts = NumVerts;
    //set the buffer data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER ,id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*numVerts, data, GL_STATIC_DRAW);
  }

  GLIndexBuffer::~GLIndexBuffer()
  {
    glDeleteBuffers(1,&id);
  }

  void GLIndexBuffer::use()
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id);
  }

  void GLIndexBuffer::unuse()
  {
  }

  void GLIndexBuffer::draw(int type)
  {
    glDrawElements(type, numVerts, GL_UNSIGNED_INT, 0);
  }

  GLIndexBufferDynamic::GLIndexBufferDynamic()
  {
    glGenBuffers(1,&id);
  }

  GLIndexBufferDynamic::~GLIndexBufferDynamic()
  {
    glDeleteBuffers(1,&id);
  }

  void GLIndexBufferDynamic::update(GLuint* data, const int NumVerts)
  {
    numVerts = NumVerts;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*numVerts, data, GL_DYNAMIC_DRAW);
  }

  void GLIndexBufferDynamic::use()
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id);
  }

  void GLIndexBufferDynamic::unuse()
  {
  }

  void GLIndexBufferDynamic::draw(int type)
  {
    glDrawElements(type, numVerts, GL_UNSIGNED_INT, 0);
  }

};
