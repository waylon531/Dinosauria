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
  return f*((2.0 * n) / (f + n - z * (f - n)));
}

#define DOF_FACTOR 1.0

float getBlur(float d)
{
  float xd = abs(d - focalLength);
  float xxd = (d < focalLength) ? (focalLength - xd) : (focalLength + xd);
  return DOF_FACTOR * (xd/xxd);
  //return DOF_FACTOR * abs(focalLength - d);
}

void main()
{
  float z = texture2D(tex_depth,vTexCoord.st).r;
  float depth = toDepth(z);
  float amount = clamp(getBlur(depth),0.0,1.0);
  fColor = mix(texture2D(tex_color,vTexCoord.st), texture2D(tex_dof,vTexCoord.st), amount);
  //fColor = vec4(vec3(depth),1.0);
  //fColor = texture2D(tex_dof,vTexCoord.st);
}
