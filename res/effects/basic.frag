#version 410

uniform sampler2D tex_color;

layout (location=0) in vec2 vTexCoord;
out vec4 fColor;

void main()
{
  fColor = texture2D(tex_color,vTexCoord.st);
}