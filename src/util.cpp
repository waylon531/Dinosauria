#include "util.hpp"

#include <ios>
using std::ios;

glm::vec3 triangleNormal(const float p00, const float p01, const float p10, const float s)
{
  glm::vec3 s0(s,p01-p00,0.f);
  glm::vec3 s1(0.f,p10-p00,s);
  //s0 = glm::normalize(s0);
  //s1 = glm::normalize(s1);
  /*glm::vec3 tmp = glm::normalize(glm::cross(glm::normalize(s0), glm::normalize(s1)));*/
  glm::vec3 n(-s0.y,s,-s1.y);
  n = glm::normalize(n);
  if(n.y<0.f) n *= -1.f;
  n = glm::vec3(n.x,n.z,n.y);
  return n;
}

void computeGradient(float* a, const int xres, const int yres, glm::vec3* tangents, glm::vec3* bitangents, glm::vec3* normals, float* value, Callback& callback)
{
  float s = 1.f/xres;
  for(int x=0; x<xres; x++)
    {
      for(int y=0; y<yres; y++)
	{
	  
	  const float s = 1.f/xres ;
	  glm::vec3 tangent(s,a[clamp(x+1,0,xres-1)*xres+y]-a[x*xres+y] ,0.f);
	  glm::vec3 bitangent(0.f,a[x*xres+clamp(y+1,0,yres-1)]-a[x*xres+y] ,s);
	  normals[x*xres+y] = glm::vec3(0.f,0.f,0.f);
	  for(int ox=-1; ox<2; ox++)
	    {
	    for(int oy=-1; oy<2; oy++)
	      {
		if((ox+oy)==1.f)
		  {
		    normals[x*xres+y] += triangleNormal(a[x*xres+y], a[clamp(x+ox,0,xres-1)*xres+y], a[x*xres+clamp(y+oy,0,yres-1)], s);
		  }
	      }
	    }
	  normals[x*xres+y] = glm::normalize(normals[x*xres+y]);
	  //std::cout << out[x*xres+y].x << "," << out[x*xres+y].y << "," << out[x*xres+y].z << std::endl;
	}
      if(x % 10 ==0)
	{
	  *value = x/(float)xres;
	  callback.call();
	}
    }
}

void computeGradient3D(float* a, const int xres, const int yres, const int zres, glm::vec3* tangents, glm::vec3* bitangents, glm::vec3* normals)
{
  float s = 1.f/xres;
  for(int x=0; x<xres; x++)
    {
      for(int y=0; y<yres; y++)
	{
	  for(int z=0; z<zres; z++)
	    {
	      const int ind = x*xres*zres+y*zres+z;
	      const float s = 1.f/xres ;
	      glm::vec3 tangent(s,a[clamp(x+1,0,xres-1)*xres*zres+y*zres+z]-a[x*xres*zres+y*zres+z] ,0.f);
	      glm::vec3 bitangent(0.f,a[x*xres*zres+clamp(y+1,0,yres-1)*zres+z]-a[x*xres*zres+y*zres+z] ,s);
	      normals[ind] = glm::vec3(0.f,0.f,0.f);
	      for(int ox=-1; ox<2; ox++)
		{
		  for(int oy=-1; oy<2; oy++)
		    {
		      if((ox+oy)==1.f)
			{
			  normals[ind] += triangleNormal(a[x*xres*zres+y*zres+y], a[clamp(x+ox,0,xres-1)*xres*zres+y*zres+z], a[x*xres*zres+clamp(y+oy,0,yres-1)*zres+z], s);
			}
		    }
		}
	      normals[ind] = glm::normalize(normals[ind]);
	      //std::cout << out[x*xres+y].x << "," << out[x*xres+y].y << "," << out[x*xres+y].z << std::endl;
	    }
	}
    }
}

void loadFileToString(std::ifstream& file, std::string& str)
{
	std::stringstream sbuf;
	sbuf << file.rdbuf();
	str = sbuf.str();
}

void dError(const std::string& message)
{
  std::cout << "[Error]: " << message << std::endl;
  raise(SIGTERM);
}
void dWarning(const std::string& message)
{
  std::cout << "[Warning]: " << message << std::endl;
#ifdef DEBUG
  sleep(1);
#endif
}

wchar_t* getWchar(const std::string& str)
{
  wchar_t* tmp = new wchar_t[str.size()];
  for(int i=0; i<str.size(); i++)
    {
      tmp[i] = str[i];
    }
  return tmp;
}

void writeFloatFile(const int xres, const int yres, const std::string& fname, float* a)
{
  FILE* f = fopen(fname.c_str(),"wb");
  fwrite(a, sizeof(float), xres*yres*sizeof(float), f);
  fclose(f);
}

float* readFloatFile(const int xres, const int yres, const std::string& fname)
{
  float* a = new float[xres*yres];
  FILE* f = fopen(fname.c_str(),"rb");
  fread(a, sizeof(float), xres*yres*sizeof(float), f);
  fclose(f); return a;
}
  
