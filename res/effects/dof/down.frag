#version 410

uniform sampler2D tex_color;

in vec2 vTexCoord;
out vec4 fColor;

void main()
{
  fColor = texture2D(tex_color,vTexCoord.st);
}
