#version 410

uniform sampler2D tex_color;
uniform sampler2D tex_depth;

layout (location=0) in vec2 vTexCoord;
out vec4 fColor;

float toDepth(float x)
{
  return -1./(x-1.00001);
}

void main()
{
  //water compositing
  vec4 color_land = texture2D(tex_color,vTexCoord.st);
  float z = texture2D(tex_depth,vTexCoord.st).x;
  float depth = toDepth(z);

#define DENSITY .001
  //float fogFactor = DENSITY*pow(depth,1.1);
  float fogFactor = depth*DENSITY;
  vec4 fog = vec4(1.0,.0,.0,1.0);

  fColor = mix(color_land, fog, clamp(fogFactor,0,1));
}
