#pragma once
/** @file graphics/buffer.hpp Handling of VBOs and GLSL attribute sets */

#include "graphics/opengl.hpp"

#include <vector>
#include <cstdlib>
#include <iostream>
#include <csignal>

namespace graphics
{
  /** @brief Class to hold information about a GLSL attribute GL_FLOAT only
   * 
   * @todo implement other types */
  class GLSLAttribute
  {
  private:
    //private data
		
    /** @brief Name of attribute
     * 
     * this is a char* because OpenGL does not use std::string */
    char* name;
		
    /** @brief Size of the attribute's data */
    int dataSize;
    /** @brief Number of GLfloats in this attribute */
    int numFloats;
  public:
    //public methods
		
    /** @brief Create the info class 
     * 
     * @param name name of attribute used in GLSL programs
     * @param sizeo size of attribute, not in bytes, but in number of glFloats*/
    GLSLAttribute(const char* name, const int size);
		
    /** @brief default constructor */
    GLSLAttribute(){}
		
    /** @brief Get the name of this attribute
     * 
     * @return GLSL name used for this attribute */
    const char* getName() const;
		
    /** @brief Get the size of this attribute
     * 
     * This is usefull because OpenGL buffers store all attributes for
     * each vertex in the same place.
     * @return Size (in bytes) of this attribute */
    const int getSize() const;
		
    /** @brief Get the number of floats in this attribute
     * 
     * @return Size (in GLfloats) of this attribute */
    const int getNumFloats() const;
		
    /** @brief Deallocate name buffer */
    ~GLSLAttribute();
  };
	
  /** @brief Wrapper for a set of OpenGL attributes
   * 
   * This is a storage of information ABOUT the attribute set, it is
   * not the actual data IN the atribute set */
  class GLSLAttributeSet
  {
  private:
    //private data
		
    /** @brief Stride of the total data for this set */
    int stride;
		
    /** @brief Set of attribute info pieces */
    std::vector<GLSLAttribute> attributes;
		
    /** @brief Set of data locations in the stride for indexed attributes */
    std::vector<int> indexedOffset;
		
  public:
    //public methods
		
    /** @brief Initialize from a vector of attributes 
     * 
     * @param Attributes std::vector containing GLSLAttributes */
    GLSLAttributeSet(std::vector<GLSLAttribute> Attributes);
		
    /** @brief default constructor */
    GLSLAttributeSet(){}
		
    /** @brief default destructor
     * 
     * This is used to avoid a strang double free bug. */
    ~GLSLAttributeSet(){}
		
    /** @brief Get the stride of this attribute set
     * 
     * @return Data stride */
    const int getStride() const;
		
    /** @brief Get the raw vector of attributes
     * 
     * @return std::vector<GLSLAttribute> for this set */
    std::vector<GLSLAttribute>& getAttributes();
		
    /** @brief Set the vertexAttribPointers for this attribute set */
    void use();
		
    /** @brief Disabe the vertexAttribPointers for this attribute set */
    void unuse();
		
    /** @brief Allocate a chunk of data for one vertex of attirbute data
     * 
     * This data will have to be dealllocated later. This will use malloc
     * because new is not an option so free must be used for deallocation.
     * @return Vertex */
    void* createVertex();
		
    /** @brief Given a vertex data chunk, get pointer to a named attribute from it
     * 
     * @param vertex vertex data
     * @param name name of attribute to get
     * @return void* pointing to the attribute 
     * @todo use hashes instead of sorting */
    void* getNamedAttribute(void* vertex, const char* name);
		
    /** @brief Given a vertex data chunk, get a pointer to an indexed attribute
     * 
     * @param vertex vertex data
     * @param index index of attribute
     * @return void* pointing to the attribute */
    void* getIndexedAttribute(void* vertex, const int index);
  };
  	
  /** @brief Wrapper for OpenGL vertex buffer */
  class GLVertexBuffer
  {
  private:
    //private data
		
    /** @brief OpenGL buffer id */
    GLuint id;
		
    /** @brief Total size (in bytes) of the data in this buffer */
    int size;
		
    /** @brief Number of vertices stored in the buffer */
    int numVerts;
		
    /** @brief Attributes used by this vertex buffer */
    GLSLAttributeSet attributes;
		
  public:
    //public data
		
    /** @brief Data stored in the buffer
     * 
     * Note that this should not be resized while this buffer is active
     * because that might change the location of the first element and
     * cause segfaults */
    void* data;
		
    //public methods
		
    /** @brief Initialize from a set of data and attr info
     * 
     * @param Data data to initialize from */
    GLVertexBuffer(void* Data, const int NumVerts, GLSLAttributeSet& Attributes);
		
    /** @brief Dealocate buffer memory from the GPU */
    ~GLVertexBuffer();
		
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

  /** @brief Wrapper for OpenGL vertex buffer */
  class GLVertexBufferDynamic
  {
  private:
    //private data
		
    /** @brief OpenGL buffer id */
    GLuint id;
		
    /** @brief Number of vertices stored in the buffer */
    int numVerts;
    	
    /** @brief Attributes used by this vertex buffer */
    GLSLAttributeSet attributes;
		
  public:
		
    //public methods
		
    /** @brief Initialize from a set of data and attr info
     * 
     * @param Data data to initialize from */
    GLVertexBufferDynamic(GLSLAttributeSet& Attributes);
		
    /** @brief Dealocate buffer memory from the GPU */
    ~GLVertexBufferDynamic();

    /** Update the buffer's data
     * @param data buffer data to use
     * @param NumVerts number of vertices */
    void update(void* data, const int NumVerts);
		
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

}
