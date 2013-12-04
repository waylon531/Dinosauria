#version 410

uniform sampler2D tex_shadow;
uniform sampler2D tex_depth;
uniform sampler2D tex_in;
uniform float t;

layout (location = 0) in vec2 vTexCoord;

out vec4 fColor;

void main()
{
  float depth = texture2D(tex_shadow, vTexCoord.st).x;
  fColor = vec4(depth,depth,depth,1.f);
}
