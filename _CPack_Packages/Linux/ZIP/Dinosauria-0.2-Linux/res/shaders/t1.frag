#version 410

uniform sampler2D tex_bricks;

layout (location = 0) in vec2 vTexCoord;
layout (location = 1) in vec3 vLocalPos;

out vec4 fColor;

void main()
{
  fColor = texture2D(tex_bricks, vTexCoord);
  //fColor = vec4(vLocalPos.xyz,1.0f);
}
