#pragma once
/** @file graphics/glsl.hpp GLSL/shader wrappers and support */

#include "graphics/opengl.hpp"
#include "graphics/glsl_unif.hpp"
#include "graphics/buffer.hpp"
#include "util.hpp"

#include <cstdlib>
#include <memory>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

namespace graphics
{
	
  /** @brief Bundle of a GLSL uniform with a GLSL location */
  typedef struct
  {
    std::shared_ptr<GLSLUniform> uniform;
    GLuint loc;
  } GLSLUniformAndLoc;
	
  /** @brief Wrapper for GLSL shader programs */
  class GLSL
  {
  private:
    //private data
		
    /** @brief OpenGL id number of shader program */
    GLuint prog_id;

    /** The name used to debug if something goes wrong */
    std::string debugName;
		
    /** @brief List of uniforms attached to this shader */
    std::vector<GLSLUniformAndLoc> uniforms;
		
    //private methods
		
    /** @brief Check the compile time log for errors 
     * 
     * @param s GLSL shader object id 
     * @param fname name of file with shader source (to identify it to user) */
    void checkCompileLog(GLuint s, std::string fname);
		
    /** @brief Check the link time log for errors
     * 
     * @param p GLSL program object id */
    void checkLinkLog(GLuint p);
		
    /** @brief Run the custom preprocessor on a GLSL source string
     * 
     * @param s source string of program 
     * @return processed source string */
    std::string ppSource(std::string s);
	
  public:
    //public methods
		
    /** @brief Compile from a vertex shader file and a fragment shader file
     * 
     * If you want to add a geometry shader, use GLSL(const string, const string, const string).
     * @param fname_vert vertex filename
     * @param fname_frag fragment filename */
    GLSL(const std::string fname_vert, const std::string fname_frag);
		
    /** @brief Compile from a vertex shader file, a geometry shader file, anda fragment shader file
     * 
     * If you want to use the fixed-function geometry shader, us GLSL(const string, const string).
     * The fixed function shader is probably faster than simply writing a passthrough
     * shader and it is definitely faster on my computer, which will 
     * use a software renderer for geometry because geometry shaders
     * are not supported on embeded cards. 
     * @param fname_vert vertex filename
     * @param fname_geom geometry filename
     * @param fname_frag fragment filename */
    GLSL(const std::string fname_vert, const std::string fname_geom, const std::string fname_frag);
		
    /** @brief Create from a OpenGL shader id number
     * 
     * For the default fixed-function shader set, use 0 as the id. This
     * is also used to copy shaders but keep in mind that if you want to
     * create a full copy, then recompile from the files instead of
     * using an already compiled shader because all changes will
     * apply to both since they both have the same id.
     * @param id OpenGL shader id */
    GLSL(const GLuint id);

    /** @brief Create a new program to use
     *
     * @param nothing a useless parameter */
    GLSL(const int nothing);

    GLSL(){}
    
    /** @brief Destroy object and free the shader on the GPU
     * 
     * Do not do this while this shader is active because it will cause
     * a crash when OpenGL does anything! */
    ~GLSL();

    /** @brief Manually attach and compile a shader
     *
     * After attaching all shaders, the linkShader() method must be called
     * @param fname file name conataining source
     * @param type type of shader to create */
    void addShader(const std::string fname, const GLuint type);

    /** @brief Manually link the shader
     *
     * All shaders must be attached before this */
    void linkShader();
    
    /** @brief Set this as the current shader
     * 
     * There can only be one active shader at a time. This will also
     * set all attached uniforms UNLESS they have been detached. This
     * is usefull because all uniforms must be set again each time the
     * shader changes and because since uniforms are stored in a pointer,
     * this will allow uniforms to be changed without directly referencing
     * this shader. */
    void use();
		
    /** @brief Attach a glsl uniform to this shader
     * 
     * After attaching this uniform, it will continue to be set each
     * time that the shader is activated until it is detached
     * @param uniform uniform to add */
    void attachUniform(std::shared_ptr<GLSLUniform> uniform);
  };
  extern std::shared_ptr<GLSLUniform> u_ortho;
  extern std::shared_ptr<GLSLUniform> u_time;
}
