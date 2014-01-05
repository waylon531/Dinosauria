#include "graphics/buffer.hpp"
using namespace graphics;

//GLSLAttribute methods

GLSLAttribute::GLSLAttribute(const char* Name, const int size, GLenum t)
{
  type = t;
  const int len = strlen(Name);
  name = new char[len];
  name = (char*)memcpy(name,Name,len*sizeof(char));
  numFloats = size;
  switch(type)
    {
    case GL_FLOAT:
      dataSize = size*sizeof(GLfloat);
      break;
    case GL_INT:
      dataSize = size*sizeof(GLfloat);
      break;
    };
}

GLSLAttribute::~GLSLAttribute()
{
	if(strcmp(name,"")!=0)
	{
		//delete name;
	}
}

const char* GLSLAttribute::getName() const
{
	return name;
}

const int GLSLAttribute::getSize() const
{
	return dataSize;
}

const int GLSLAttribute::getNumFloats() const
{
	return numFloats;
}

//GLSLAttributeSet methods

GLSLAttributeSet::GLSLAttributeSet(std::vector<GLSLAttribute> Attributes)
{
	attributes = Attributes;
	indexedOffset = std::vector<int>();
	//calculate stride
	stride = 0;
	for(std::vector<GLSLAttribute>::iterator attribute
		= attributes.begin();
		attribute != attributes.end();
		++attribute)
	{
		indexedOffset.push_back(stride);
		stride += attribute->getSize();
	}
}

const int GLSLAttributeSet::getStride() const
{
	return stride;
}

std::vector<GLSLAttribute>& GLSLAttributeSet::getAttributes()
{
	return attributes;
}

void GLSLAttributeSet::use()
{
	for(int ind=0; ind<attributes.size(); ind++)
	{
		glEnableVertexAttribArray(ind);
		glVertexAttribPointer(ind, 
				attributes.at(ind).getNumFloats(), //number of GLfloats
				attributes[ind].type, //type
				GL_FALSE, //do not normalize
				stride, //stride
				(void*)indexedOffset.at(ind)); //offset
	}
}

void GLSLAttributeSet::unuse()
{
	for(int ind=0; ind<attributes.size(); ind++)
	{
		glDisableVertexAttribArray(ind);
	}
}
				

void* GLSLAttributeSet::createVertex()
{
	return malloc(stride);
}

void* GLSLAttributeSet::getNamedAttribute(void* vertex, const char* name)
{
	//go through until "name" is found
	for(int ind=0; ind<attributes.size(); ind++)
	{
		if(strcmp(name,attributes.at(ind).getName()) == 0)
		{
			return getIndexedAttribute(vertex,ind);
		}
	}
	std::cout << name << " is not a valid attribute" << std::endl;
	raise(SIGTERM);
	return NULL;
}

void* GLSLAttributeSet::getIndexedAttribute(void* vertex, const int index)
{
	return vertex + indexedOffset.at(index);
}

//GLBuffer methods

GLVertexBuffer::GLVertexBuffer(void* Data, const int NumVerts, GLSLAttributeSet& Attributes)
{
	data = Data;
	glGenBuffers(1,&id);
	numVerts = NumVerts;
	attributes = Attributes;
	size = attributes.getStride() * numVerts;
	//set the buffer data
	glBindBuffer(GL_ARRAY_BUFFER,id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

GLVertexBuffer::~GLVertexBuffer()
{
	glDeleteBuffers(1,&id);
}

void GLVertexBuffer::use()
{
	glBindBuffer(GL_ARRAY_BUFFER,id);
	attributes.use();
}

void GLVertexBuffer::unuse()
{
	attributes.unuse();
}

void GLVertexBuffer::draw(int type)
{
	glDrawArrays(type, 0, numVerts);
}

GLVertexBufferDynamic::GLVertexBufferDynamic(GLSLAttributeSet& Attributes)
{
  attributes = Attributes;
  glGenBuffers(1,&id);
}

GLVertexBufferDynamic::~GLVertexBufferDynamic()
{
  glDeleteBuffers(1,&id);
}

void GLVertexBufferDynamic::update(void* data, const int NumVerts)
{
  numVerts = NumVerts;
  int size = attributes.getStride() * numVerts;
  glBindBuffer(GL_ARRAY_BUFFER,id);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void GLVertexBufferDynamic::use()
{
  glBindBuffer(GL_ARRAY_BUFFER,id);
  attributes.use();
}

void GLVertexBufferDynamic::unuse()
{
  attributes.unuse();
}

void GLVertexBufferDynamic::draw(int type)
{
  glDrawArrays(type, 0, numVerts);
}
