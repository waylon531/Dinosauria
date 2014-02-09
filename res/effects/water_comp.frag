#version 410

uniform sampler2D tex_color;
uniform sampler2D tex_depth;
uniform sampler2D tex_color_reverse;
uniform sampler2D tex_depth_reverse;
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

#define REFLECTION_SCALE 0.1
#define REFRACTION_STEP 0.01
#define MAX_REFRACT 5.0
#define REFRACTION_SCALE 0.05
#define R_MIN 0.2

float toDepth(float z)
{
  float n = .5; // camera z near
  float f = 200.0; // camera z far
  return f*((2.0 * n) / (f + n - z * (f - n)));
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
	  vec4 color_land_reflect = vec4(0.0,0.0,0.0,1.0);
	  vec3 coord = vec3(vTexCoord.st,0.f) + (m_project * m_view * REFLECTION_SCALE * vec4(wNormal,0.f)).xyz;
	  //float z2 = texture2D(tex_depth_reverse, coord.st);
	  color_land_reflect = texture2D(tex_color_reverse, coord.st);
	  //refraction
	  {
	    vec3 dir=refract(eyeDir, wNormal, 1.0/1.4);
	    dir = normalize(dir);
	    vec3 coord2 = vec3(vTexCoord.st,0.f) + REFRACTION_SCALE*(m_project * m_view * vec4(dir,0.f)).xyz;
	    float zz = texture2D(tex_depth, coord2.st);
	    if(zz > water_z)
	      {
		color_land = texture2D(tex_color, coord2.st);
		z = zz;
	      }
	  }
	  //z = z2;
	  float depth = toDepth(z);
	  fDepth = vec4(z);
	  vec4 color_water = texture2D(tex_water_color,vTexCoord.st).rgba;
	  vec4 color_depth = mix(color_land,vec4(0.0,0.0,.5,1.0), clamp(0.01*(depth-water_depth),0.0,1.0));
	  float tmpC = abs(1.0 - dot(normalize(wNormal), -normalize(eyeDir-eyePos)));
	  float fresnelTerm = R_MIN + (1.0 - R_MIN) * tmpC*tmpC*tmpC*tmpC*tmpC;
	  //fColor = vec4(vec3(fresnelTerm),1.f);
	  //return;
	  fColor = mix(color_depth, mix(color_water,color_land_reflect,fresnelTerm), 0.5);
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
	  fColor = mix(mix(color_land,vec4(0.0,0.0,0.5,1.0), clamp(0.1+0.01*(water_depth),0.0,1.0)), color_water, 0.25);
	}
      else
	{
	  fColor = mix(color_land,vec4(0.0,0.0,0.5,1.0), clamp(0.1+0.01*(depth),0.0,1.0));
	  fDepth = vec4(z);
	}
    }      
}
