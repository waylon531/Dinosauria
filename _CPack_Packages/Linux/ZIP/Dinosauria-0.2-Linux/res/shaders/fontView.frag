#version 410

uniform sampler2D tex0;

layout (location=0) in vec2 vTexCoord;

out vec4 fColor;

void main()
{
  float alpha = texture2D(tex0, vTexCoord.st).a;
  fColor = vec4(alpha,alpha,alpha,1);
}
