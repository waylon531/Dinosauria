#version 410

uniform sampler2D tex_color;
uniform sampler2D tex_dof;
uniform sampler2D tex_depth;
uniform float focalLength;

in vec2 vTexCoord;
out vec4 fColor;


float toDepth(float z)
{
  float n = .5; // camera z near
  float f = 200.0; // camera z far
  return .5*f*((2.0 * n) / (f + n - z * (f - n)));
}

#define DOF_FACTOR 0.5
#define DEADBAND 0.01

float getBlur(float d)
{
  float xd = abs((d - focalLength)*DOF_FACTOR);
  float xxd = (d < focalLength) ? DOF_FACTOR*(focalLength - xd) : DOF_FACTOR*(focalLength + xd);
  return (xd/xxd);
  //return DOF_FACTOR * abs(focalLength - d);
}

void main()
{
  //fColor = texture2D(tex_color, vTexCoord.st);
  //return;
  float z = texture2D(tex_depth,vTexCoord.st).r;
  float depth = toDepth(z);
  float amount = clamp(getBlur(depth),0.0,1.0);
  if(amount < DEADBAND) amount = 0.f;
  fColor = mix(texture2D(tex_color,vTexCoord.st), texture2D(tex_dof,vTexCoord.st), amount);
  //fColor = vec4(vec3(depth),1.0);
  //fColor = texture2D(tex_dof,vTexCoord.st);
}
