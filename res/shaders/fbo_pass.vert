#version 410

layout (location = 0) in vec2 position;

out vec2 vTexCoord;

void main()
{
  gl_Position = vec4(position,0.0,1.0);
  vTexCoord = (position + 1.0)/2.0;
}
