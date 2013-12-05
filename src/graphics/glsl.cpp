#include "graphics/glsl.hpp"
using namespace graphics;

glm::mat4 gmat_ortho = glm::ortho(0.0f,(float)W,0.0f,(float)H);
std::shared_ptr<graphics::GLSLUniform> graphics::u_ortho(new graphics::GLSLUniform("m_ortho",&gmat_ortho));
std::shared_ptr<graphics::GLSLUniform> graphics::u_time;

//GLSL methods

GLSL::GLSL(const std::string fname_vert, const std::string fname_frag)
{	
  prog_id = glCreateProgram();

  addShader(fname_vert, GL_VERTEX_SHADER);
  addShader(fname_frag, GL_FRAGMENT_SHADER);
  linkShader();
  
  //init vectors
  uniforms = std::vector<GLSLUniformAndLoc>();
}

GLSL::GLSL(const std::string fname_vert, const std::string fname_geom, const std::string fname_frag)
{
  prog_id = glCreateProgram();

  addShader(fname_vert, GL_VERTEX_SHADER);
  addShader(fname_geom, GL_GEOMETRY_SHADER);
  addShader(fname_frag, GL_FRAGMENT_SHADER);
  linkShader();	
  //init vectors
  uniforms = std::vector<GLSLUniformAndLoc>();
}

GLSL::GLSL(GLuint id)
{
  prog_id = id;
  //check for errors
  glUseProgram(prog_id);
	
  //init vectors
  uniforms = std::vector<GLSLUniformAndLoc>();
}

GLSL::GLSL(const int nothing)
{
  prog_id = glCreateProgram();
  uniforms = std::vector<GLSLUniformAndLoc>();
}

GLSL::~GLSL()
{
  if(prog_id != 0)
    {
      glDeleteProgram(prog_id);
    }
}

void GLSL::addShader(const std::string fname, const GLuint type)
{
  debugName = fname;
  GLuint id = glCreateShader(type);
#ifdef DEBUG
  std::cout << "Created shader of type " << type << " with id " << id << std::endl;
#endif
  std::ifstream f(fname);
  if(!f.good())
    {
      std::cout << "Error opening shader file: " << fname << std::endl;
      return;
    }
  std::string src;
  loadFileToString(f,src);
  src = ppSource(src);
  f.close();
  const char* char_src = src.c_str();
  glShaderSource(id, 1, &char_src, NULL);
  glCompileShader(id);
  checkCompileLog(id,fname);
  glAttachShader(prog_id,id);
  glDeleteShader(id);
}

void GLSL::linkShader()
{
  glLinkProgram(prog_id);
  checkLinkLog(prog_id);
  glUseProgram(prog_id);
}

std::string GLSL::ppSource(std::string s)
{
  //my custom GLSL preprocessor
  //all directives for this should be in the format of a comment, followed
  //by a backslash, then the directive: eg. "//\include <thing.slh>
  std::stringstream out;
  std::stringstream in(s);
  std::string line;
  std::string strtmp;
  std::ifstream f;
  while(std::getline(in,line))
    {
      if(line.substr(0,3) == "//\\")
	{
	  //inclue directive
	  if(line.substr(0,10) == "//\\include")
	    {
	      f.open(line.substr(11));
	      assert(f.good() && "Error reading included file");
	      loadFileToString(f,strtmp);
	      out << ppSource(strtmp);
	      f.close();
	    }
	}
    }
  out << s;
  return out.str();
}

void GLSL::checkCompileLog(GLuint s, std::string fname)
{
  GLint status;
  glGetShaderiv(s,GL_COMPILE_STATUS,&status);
  if(status != GL_TRUE)
    {
      std::cout << "[GLSL Error status=" << status << "]: shader compile failed! (" << fname << ")" << std::endl;
      //get the error log
      int infologLength = 0;
      int charsWritten  = 0;
      char *infoLog;
	
      glGetShaderiv(s, GL_INFO_LOG_LENGTH,&infologLength);
      infoLog = (char *)malloc(infologLength);
      glGetShaderInfoLog(s, infologLength, &charsWritten, infoLog);
      std::cout << infoLog << std::endl;
      free(infoLog);
    }
}

void GLSL::checkLinkLog(GLuint p)
{
  GLint status;
  glGetProgramiv(p,GL_LINK_STATUS,&status);
  if(status != GL_TRUE)
    {
      std::cout << "[GLSL Error]: program link failed!" << std::endl;
      //get the error log
      int infologLength = 0;
      int charsWritten  = 0;
      char *infoLog;
	
      glGetProgramiv(p, GL_INFO_LOG_LENGTH,&infologLength);
	
      if (infologLength > 0)
	{
	  infoLog = (char *)malloc(infologLength);
	  glGetProgramInfoLog(p, infologLength, &charsWritten, infoLog);
	  std::cout << infoLog << std::endl;
	  free(infoLog);
	}
      raise(SIGTERM);
    }
}

void GLSL::use()
{
  glUseProgram(prog_id);
  //set uniforms
  for(std::vector<GLSLUniformAndLoc>::iterator uniform
	= uniforms.begin();
      uniform != uniforms.end(); ++uniform)
    {
      uniform->uniform->use(uniform->loc);
    }
}

void GLSL::attachUniform(std::shared_ptr<GLSLUniform> uniform)
{
  GLint loc = glGetUniformLocation(prog_id, uniform->getName().c_str());
  if(loc==-1) std::cout << "GLSL Error: could not find uniform \"" << uniform->getName() << "\" in shader " << debugName << std::endl;
  #ifdef DEBUG
  std::cout << "Adding uniform \"" << uniform->getName() << "\" to shader " << debugName << " with location " << loc << std::endl;
  #endif
  uniforms.push_back(GLSLUniformAndLoc{uniform,(GLuint)loc});
}
