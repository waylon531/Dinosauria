#version 410

layout (location=0) in vec2 position;
out vec2 vTexCoord;

void main()
{
  vTexCoord = (position+1.0)/2.0;
  vTexCoord.xy = vTexCoord.yx;
  vTexCoord.x = 1-vTexCoord.x;
  gl_Position = vec4(position,0.0,1.0);
}
