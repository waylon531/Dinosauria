#version 410

uniform mat4 m_project;
uniform mat4 m_view;
uniform mat4 m_model;

layout (location=0) in vec3 position;
layout (location=3) in ivec4 boneIds;
layout (location=4) in vec4 boneWeights;

void main()
{
  gl_Position = m_project * m_view * m_model * vec4(position,1.f);
}
