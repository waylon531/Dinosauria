#pragma once
/** @file graphics/glsl_unif.hpp GLSL uniform wrapper and handler */

#include "graphics/opengl.hpp"

#include <iostream>
#include <csignal>

namespace graphics
{
	
  /** @brief GLSL uniform types
   * 
   * This enum helps keep all uniforms in one class */
  enum enum_uniformTypes
    {
      UNIFORM_INT,
      UNIFORM_UINT,
      UNIFORM_FLOAT,
      UNIFORM_VEC2,
      UNIFORM_VEC3,
      UNIFORM_VEC4,
      UNIFORM_MAT2,
      UNIFORM_MAT3,
      UNIFORM_MAT4,
      UNIFORM_BOOL
    };
	
  /** @brief GLSL uniform wrapper 
   * 
   * It supports all glm types and int/float/bools but has no derived
   * classes for them to avoid overhead from virtual functions.*/
  class GLSLUniform
  {
  private:
    //private data
		
    /** @brief Name of uniform in GLSL program */
    std::string name;
	  
  public:
    //public methods
		
    /** @brief Initialize as integer
     * 
     * @param Name name of uniform 
     * @param Value value of uniform */
    GLSLUniform(const std::string& Name, GLint* Value );
		
    /** @brief Initialize as uint
     * 
     * @param Name name of uniform 
     * @param Value value of uniform */
    GLSLUniform(const std::string& Name, GLuint* Value );
		
    /** @brief Initialize as float
     * 
     * @param Name name of uniform 
     * @param Value value of uniform */
    GLSLUniform(const std::string& Name, GLfloat* Value );
		
    /** @brief Initialize as vec2
     * 
     * @param Name name of uniform 
     * @param Value value of uniform */
    GLSLUniform(const std::string& Name, glm::vec2* Value );
		
    /** @brief Initialize as vec3
     * 
     * @param Name name of uniform 
     * @param Value value of uniform */
    GLSLUniform(const std::string& Name, glm::vec3* Value );
		
    /** @brief Initialize as vec4
     * 
     * @param Name name of uniform 
     * @param Value value of uniform */
    GLSLUniform(const std::string& Name, glm::vec4* Value );
		
    /** @brief Initialize as mat2
     * 
     * @param Name name of uniform 
     * @param Value value of uniform */
    GLSLUniform(const std::string& Name, glm::mat2* Value );
		
    /** @brief Initialize as mat3
     * 
     * @param Name name of uniform 
     * @param Value value of uniform */
    GLSLUniform(const std::string& Name, glm::mat3* Value );
		
    /** @brief Initialize as mat4
     * 
     * @param Name name of uniform 
     * @param Value value of uniform */
    GLSLUniform(const std::string& Name, glm::mat4* Value );

    /** Initialize as boolean
     * @param Name name of uniform
     * @param Value value pointer */
    GLSLUniform(const std::string& Name, bool* Value);
    
    /** @brief Destructor */
    ~GLSLUniform();

    /** @brief Type of uniform that this contains */
    enum_uniformTypes type;
	  
    /** @brief Value
     * 
     * This is stored in a void* but will be cast into the correct value
     * whenever it is accessed */
    void* value;
		
    /** @brief Set this uniform (given a location)
     * 
     * @param location location of GLSL uniform in program */
    void use(const GLint location);
		
    /** @brief Get the name of this uniform
     * 
     * This is a std::string& because the std::string is a class and not
     * compatible with C OpenGL functions.
     * @return name of the uniform */
    std::string getName() const;

    void setName(const std::string&);
		
    /** @brief Get pointer to integer value
     * 
     * @return value */
    GLint* getInt();
		
		
    /** @brief Get pointer to unsigned integer value
     * 
     * @return value */
    GLuint* getUint();
		
		
    /** @brief Get pointer to float value
     * 
     * @return value */
    GLfloat* getFloat();
		
    /** @brief Get pointer to vec2 value
     * 
     * @return value */
    glm::vec2* getVec2();
		
    /** @brief Get pointer to vec3 value
     * 
     * @return value */
    glm::vec3* getVec3();
		
    /** @brief Get pointer to vec4 value
     * 
     * @return value */
    glm::vec4* getVec4();
		
    /** @brief Get pointer to 2x2 matrix value
     * 
     * @return value */
    glm::mat2* getMat2();
		
    /** @brief Get pointer to 3x3 matrix value
     * 
     * @return value */
    glm::mat3* getMat3();
		
    /** @brief Get pointer to 4x4 matrix value
     * 
     * @return value */
    glm::mat4* getMat4();
  };
	
}
