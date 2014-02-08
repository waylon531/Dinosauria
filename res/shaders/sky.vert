#version 410

uniform mat4 m_view;
uniform mat4 m_project;

in vec3 position;
in vec3 normal;
in vec2 texCoord;
in vec3 tangent;

out vec3 vPosition;

void main()
{
  gl_Position = vec4((m_project * m_view * vec4(position,0.f)).xyz,1.f);
  vPosition = position;
}
