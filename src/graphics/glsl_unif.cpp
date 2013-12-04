#include "graphics/glsl_unif.hpp"
using namespace graphics;

GLSLUniform::GLSLUniform(const std::string& Name, GLint* Value)
{
	value = (void*)Value;
	name = Name;
	type = UNIFORM_INT;
}

GLSLUniform::GLSLUniform(const std::string& Name, GLuint* Value)
{
	value = (void*)Value;
	name = Name;
	type = UNIFORM_UINT;
}

GLSLUniform::GLSLUniform(const std::string& Name, GLfloat* Value)
{
	value = (void*)Value;
	name = Name;
	type = UNIFORM_FLOAT;
}

GLSLUniform::GLSLUniform(const std::string& Name, glm::vec2* Value)
{
	value = (void*)Value;
	name = Name;
	type = UNIFORM_VEC2;
}

GLSLUniform::GLSLUniform(const std::string& Name, glm::vec3* Value)
{
	value = (void*)Value;
	name = Name;
	type = UNIFORM_VEC3;
}

GLSLUniform::GLSLUniform(const std::string& Name, glm::vec4* Value)
{
	value = (void*)Value;
	name = Name;
	type = UNIFORM_VEC4;
}

GLSLUniform::GLSLUniform(const std::string& Name, glm::mat2* Value)
{
	value = (void*)Value;
	name = Name;
	type = UNIFORM_MAT2;
}

GLSLUniform::GLSLUniform(const std::string& Name, glm::mat3* Value)
{
	value = (void*)Value;
	name = Name;
	type = UNIFORM_MAT3;
}

GLSLUniform::GLSLUniform(const std::string& Name, glm::mat4* Value)
{
	value = (void*)Value;
	name = Name;
	type = UNIFORM_MAT4;
}

GLSLUniform::~GLSLUniform()
{
	//I might need to destroy the value, I am not sure
}

std::string GLSLUniform::getName() const
{
  return name;
}

void GLSLUniform::setName(const std::string& n)
{
  name = n;
}

void GLSLUniform::use(const GLint loc)
{
	switch(type)
	{
		case UNIFORM_INT:
			glUniform1i(loc, *(GLint*)value);
			break;
		case UNIFORM_UINT:
			glUniform1ui(loc, *(GLuint*)value);
			break;
		case UNIFORM_FLOAT:
			glUniform1f(loc, *(GLfloat*)value);
			break;
		case UNIFORM_VEC2:
			glUniform2fv(loc, 1, glm::value_ptr(*(glm::vec2*)value));
			break;
		case UNIFORM_VEC3:
			glUniform3fv(loc, 1, glm::value_ptr(*(glm::vec3*)value));
			break;
		case UNIFORM_VEC4:
			glUniform4fv(loc, 1, glm::value_ptr(*(glm::vec4*)value));
			break;
		case UNIFORM_MAT2:
			glUniformMatrix2fv(loc, 1, GL_FALSE, &(*(glm::mat2*)value)[0][0]);
			break;
		case UNIFORM_MAT3:
			glUniformMatrix3fv(loc, 1, GL_FALSE, &(*(glm::mat3*)value)[0][0]);
			break;
		case UNIFORM_MAT4:
			glUniformMatrix4fv(loc, 1, GL_FALSE, &(*(glm::mat4*)value)[0][0]);
			break;
		default:
			std::cout << type << " is not a valid uniform type (this could be a internal error in the wrappers or a failure to construct this properly" << std::endl;
			raise(SIGTERM);
	}
}

GLint* GLSLUniform::getInt()
{
	return (GLint*)value;
}

GLuint* GLSLUniform::getUint()
{
	return (GLuint*)value;
}

GLfloat* GLSLUniform::getFloat()
{
	return (GLfloat*)value;
}

glm::vec2* GLSLUniform::getVec2()
{
	return (glm::vec2*)value;
}

glm::vec3* GLSLUniform::getVec3()
{
	return (glm::vec3*)value;
}

glm::vec4* GLSLUniform::getVec4()
{
	return (glm::vec4*)value;
}

glm::mat2* GLSLUniform::getMat2()
{
	return (glm::mat2*)value;
}

glm::mat3* GLSLUniform::getMat3()
{
	return (glm::mat3*)value;
}

glm::mat4* GLSLUniform::getMat4()
{
	return (glm::mat4*)value;
}
