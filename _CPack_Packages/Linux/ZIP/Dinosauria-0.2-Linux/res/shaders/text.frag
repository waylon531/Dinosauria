#version 410
uniform sampler2D tex0;

layout (location=1) in vec2 vTexCoord;
layout (location=2) in vec4 vColor;
layout (location=0) in vec3 vPosition;

out vec4 fColor;

bool isNearEdge(float x)
{
#define M 0.1
  return (x < M) || (x > 1.-M);
}

void main()
{
  fColor = vec4(vColor.xyz, texture2D(tex0, vTexCoord.st).a);
  //fColor = vec4(vTexCoord.xy, texture2D(tex0,vTexCoord.xy).a, .75f);
  //fColor = vec4(1,1,1,1);
}
