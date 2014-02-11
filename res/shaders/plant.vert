#version 410

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_project;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec3 vNormal;
out vec3 vPosition;

void main()
{
  gl_Position = m_project * m_view * m_model * vec4(position, 1.f);
  vPosition = gl_Position.xyz;
  vNormal = (m_model * vec4(normal.xyz,0.f)).xyz;
}
