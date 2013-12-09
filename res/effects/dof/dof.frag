#version 410

uniform sampler2D tex_color;
uniform sampler2D tex_dof;
uniform sampler2D tex_depth;
uniform float focalLength;

in vec2 vTexCoord;
out vec4 fColor;


float toDepth(float x)
{
  return -1./(x-1.00001);
}

#define DOF_FACTOR 1.0

float getBlur(float d)
{
  float xd = abs(d - focalLength);
  float xxd = (d < focalLength) ? (focalLength - xd) : (focalLength + xd);
  return DOF_FACTOR * pow(xd/xxd,2.0);
}

void main()
{
  float z = texture2D(tex_depth,vTexCoord.st).r;
  float depth = toDepth(z);
  float amount = clamp(getBlur(depth),0.0,1.0);
  fColor = mix(texture2D(tex_color,vTexCoord.st), texture2D(tex_dof,vTexCoord.st), amount);
  //fColor = texture2D(tex_dof,vTexCoord.st);
}
