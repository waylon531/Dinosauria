#version 410

uniform mat4 m_project;
uniform mat4 m_view;
uniform mat4 m_model;

layout (location=0) in vec3 position;

void main()
{
  gl_Position = m_project * m_view * m_model * vec4(position,1.f);
}
