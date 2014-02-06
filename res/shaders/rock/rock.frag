#version 410

in vec3 vObjPosition;
in vec3 vPosition;

layout (location=0) out vec4 fColor;
layout (location=1) out vec4 fNormal;
layout (location=2) out vec4 fPosition;
layout (location=3) out vec4 fParams;

void main()
{
  fColor = vec4(.5,.5,.6,1.);
  fNormal = vec4(normalize(vObjPosition),1.f);
  fPosition = vec4(vPosition,1.f);
  fParams = vec4(.8,.8,10,1.f);
}
