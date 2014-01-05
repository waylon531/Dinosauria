#version 410

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_project;

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 texCoord;
layout (location=3) in ivec4 boneIds;
layout (location=4) in vec4 boneWeights;

out vec3 vNormal;
out vec2 vTexCoord;
out vec4 vPosition;
out vec4 vPositionView;

void main()
{
  mat4 PVM = m_project*m_view*m_model;
  gl_Position = PVM*vec4(position,1.f);
  vPosition = m_model*vec4(position,1.f);
  vPositionView = gl_Position;
  vNormal = (m_model * vec4(normal,0.f)).xyz;
  vTexCoord = texCoord;
}
