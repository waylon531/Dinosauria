#version 410

uniform sampler2D tex_color;
uniform sampler2D tex_bloom;

in vec2 vTexCoord;
out vec4 fColor;

void main()
{
  //fColor =  clamp(texture2D(tex_bloom,vTexCoord.st) / (64*64*64), vec4(0.f), vec4(1.f));
  //return;
  fColor = texture2D(tex_color, vTexCoord.st);
  return;
  fColor = clamp(texture2D(tex_bloom,vTexCoord.st) / (64.0*64.0*64.0),vec4(0.0f),vec4(1.0f)) + texture2D(tex_color,vTexCoord.st) * 1.0;
}
