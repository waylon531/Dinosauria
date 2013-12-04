#version 410
//the standard shader

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_project;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 vNormal;
out vec2 vTexCoord;

void main()
{
  mat4 m = m_project * m_view * m_model;
  gl_Position = m * vec4(position,1.0f);
  vNormal = normalize((m_view * m_model * vec4(normal,0.0f)).xyz);
  vTexCoord = texCoord;
}
