#version 410

uniform sampler2D tex_color;
uniform sampler2D tex_depth;
uniform sampler2D tex_water_color;
uniform sampler2D tex_water_depth;
uniform sampler2D tex_water_normal;
uniform vec3 eyeDir;
uniform vec3 eyePos;
uniform mat4 m_view;
uniform mat4 m_project;

layout (location=0) in vec2 vTexCoord;
layout (location=0) out vec4 fColor;
layout (location=1) out vec4 fDepth;

float toDepth(float x)
{
  return -1./(x-1.00001);
}

void main()
{
  //water compositing
  vec4 color_land = texture2D(tex_color,vTexCoord.st);
  float z = texture2D(tex_depth,vTexCoord.st);
  float depth = toDepth(z);
  float water_z = texture2D(tex_water_depth,vTexCoord.st).r;
  float water_depth = toDepth(water_z);
  if(eyePos.y > 0.0f)
    {
      if(water_z < z)
	{
	  //ULTRA-FAKE refraction
	  vec3 wNormal = texture2D(tex_water_normal, vTexCoord.st).xyz;
	  vec3 eNormal = (m_view * vec4(wNormal,1.0)).xyz;
	  vec2 coord = vTexCoord.st + 0.001*(m_project * m_view * vec4(refract(eyeDir,wNormal,1.0/1.4),1.0)).xy;
	  float z2 = texture2D(tex_depth, coord);
	  if(water_z < z2)
	    {
	      color_land = texture2D(tex_color, coord);
	      z = z2;
	    }
	  //vec4 color_water = vec4(coord,1.0,1.0);
	  float depth = toDepth(z);
	  fDepth = vec4(z);
	  vec4 color_water = texture2D(tex_water_color,vTexCoord.st).rgba;
	  fColor = mix(mix(color_land,vec4(0.0,0.0,1.0,1.0), clamp(0.001*(depth-water_depth),0.0,1.0)), color_water, 0.25);
	}
      else
	{
	  fColor = color_land;
	  fDepth = vec4(z);
	}
    }
  else
    {
      if(water_z < z)
	{
	  //ULTRA-FAKE refraction
	  vec3 wNormal = texture2D(tex_water_normal, vTexCoord.st).xyz;
	  vec3 eNormal = (m_view * vec4(wNormal,1.0)).xyz;
	  vec2 coord = vTexCoord.st + 0.001*(m_project * m_view * vec4(refract(eyeDir,wNormal,1.0/1.4),1.0)).xy;
	  float z2 = texture2D(tex_depth, coord);
	  if(water_z < z2)
	    {
	      color_land = texture2D(tex_color, coord);
	      z = z2;
	    }
	  //vec4 color_water = vec4(coord,1.0,1.0);
	  float depth = toDepth(z);
	  fDepth = vec4(z);
	  vec4 color_water = texture2D(tex_water_color,vTexCoord.st).rgba;
	  fColor = mix(mix(color_land,vec4(0.0,0.0,1.0,1.0), clamp(0.01*(water_depth),0.0,1.0)), color_water, 0.25);
	}
      else
	{
	  fColor = mix(color_land,vec4(0.0,0.0,1.0,1.0), clamp(0.01*(depth),0.0,1.0));
	  fDepth = vec4(z);
	}
    }      
}
