#pragma once
/** @file ibo.hpp OpenGL index buffers */

#include "graphics/opengl.hpp"

namespace graphics
{
    /** @brief Wrapper for OpenGL index buffer */
  class GLIndexBuffer
  {
  private:
    //private data
		
    /** @brief OpenGL buffer id */
    GLuint id;
		
    /** @brief Number of vertices stored in the buffer */
    int numVerts;
    
  public:
    //public data
		
    /** @brief Data stored in the buffer */
    GLuint* data;
		
    //public methods
    
    /** @brief Initialize from a set of data and attr info
     * 
     * @param Data data of index draws
     * @param NumVerts number of indices */
    GLIndexBuffer(GLuint* Data, const int NumVerts);
    
    /** @brief Dealocate buffer memory from the GPU */
    ~GLIndexBuffer();
    
    /** @brief Set this buffer to be active in OpenGL
     * 
     * Note that only one buffer can be active at one time. */
    void use();
    
    /** @brief Undo any settings that were changed when use() was called */
    void unuse();
    
    /** @brief Draw the buffer
     * 
     * @param type OpenGL primitive type */
    void draw(int type);
		
  };

  /** @brief Wrapper for OpenGL index buffer with dynamic data */
  class GLIndexBufferDynamic
  {
  private:
    //private data
		
    /** @brief OpenGL buffer id */
    GLuint id;
		
    /** @brief Number of vertices stored in the buffer */
    int numVerts;
    
  public:
    //public methods
    
    /** @brief Initialize */
    GLIndexBufferDynamic();
    
    /** @brief Dealocate buffer memory from the GPU */
    ~GLIndexBufferDynamic();

    /** Update buffer data
     * @param data buffer data to use
     * @param NumVerts number of primitives stored */
    void update(GLuint* data, const int NumVerts);
    
    /** @brief Set this buffer to be active in OpenGL
     * 
     * Note that only one buffer can be active at one time. */
    void use();
    
    /** @brief Undo any settings that were changed when use() was called */
    void unuse();
    
    /** @brief Draw the buffer
     * 
     * @param type OpenGL primitive type */
    void draw(int type);
		
  };

};
