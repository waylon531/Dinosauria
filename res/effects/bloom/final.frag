#version 410

uniform sampler2D tex_color_bloom;
uniform sampler2D tex_color;

in vec2 vTexCoord;
out vec4 fColor;

void main()
{
  fColor = texture2D(tex_color_bloom,vTexCoord.st) * 1.0 + texture2D(tex_color,vTexCoord.st) * 0.0;
}
