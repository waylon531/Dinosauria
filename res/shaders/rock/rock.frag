#version 410

uniform mat4 m_model;

in vec3 vObjPosition;
in vec3 vPosition;

out vec4 fColor;
out vec4 fNormal;
out vec4 fPosition;
out vec4 fParams;

void main()
{
  fColor = vec4(.5,.5,.6,1.);
  fNormal = vec4((m_model * vec4(normalize(vObjPosition),0.f)).xyz,1.f);
  fPosition = vec4(vPosition,1.f);
  fParams = vec4(1.,.0,10,1.f);
}
