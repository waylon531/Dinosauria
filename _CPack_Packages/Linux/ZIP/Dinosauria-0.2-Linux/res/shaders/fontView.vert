#version 410

layout (location=0) in vec2 pos;
layout (location=1) in vec2 texCoord;

out vec2 vTexCoord;

void main()
{
  vTexCoord = texCoord;
  vTexCoord.y = 1-vTexCoord.y;
  gl_Position = vec4(pos,0.0f,1.0f);
}
