#version 410

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_project;
uniform mat4 boneTransforms[32];

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 texCoord;
layout (location=3) in ivec4 boneIds;
layout (location=4) in vec4 boneWeights;

out vec3 vObjPosition;
out vec3 vPosition;

void main()
{
  gl_Position = (m_project * m_view * m_model) * vec4(position,1.f);
  vObjPosition = position;
  vPosition = (m_model * vec4(position,1.f)).xyz;
}
