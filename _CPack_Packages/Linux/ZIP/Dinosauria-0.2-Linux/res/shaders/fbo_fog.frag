#version 410

uniform sampler2D tex_in;
uniform sampler2D tex_depth;
uniform sampler2D tex_water_pass0;
uniform sampler2D tex_water_depth;
uniform sampler2D tex_water_pass_n;
uniform float t;

layout (location=0) in vec2 vTexCoord;
out vec4 fColor;

float toDepth(float x)
{
  return -1./(x-1.00001);
}

void main()
{
  //water compositing
  vec4 color_land = texture2D(tex_in,vTexCoord.st);
  float z = texture2D(tex_depth,vTexCoord.st);
  float depth = toDepth(z);

#define DENSITY .001
  //float fogFactor = DENSITY*pow(depth,1.1);
  float fogFactor = depth*DENSITY;
  vec4 fog = vec4(1.0,.0,.0,1.0);
  
  float water_z = texture2D(tex_water_depth,vTexCoord.st).r;
  float water_depth = toDepth(water_z);
  if(water_z < z)
    {
      vec3 wNormal = texture2D(tex_water_pass_n, vTexCoord.st).xyz;
      vec2 rOffset = wNormal.xz * 0.25;
      color_land = texture2D(tex_in, vTexCoord.st+rOffset);
      color_land = mix(color_land, fog, clamp(fogFactor,0,1));
      vec4 color_water = texture2D(tex_water_pass0,vTexCoord.st).rgba;
      //fColor = vec4(vec3(0.01*water_depth),1.0);
      fColor = mix(color_land,color_water, 0.4+clamp(0.03*(depth-water_depth),0.0,1.0));
      fColor = vec4(wNormal,1.f);
    }
  else
    {
      fColor = color_land;
    }
}
