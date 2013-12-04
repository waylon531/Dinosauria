#version 410

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_project;

layout (location=0) in vec3 position;
layout (location=1) in vec2 texCoord;

out vec2 vTexCoord;
out vec3 vLocalPos;

void main()
{
  vLocalPos = position;
  mat4 m = m_project*m_view*m_model;
  gl_Position = m * vec4(vLocalPos,1.0f);
  vTexCoord = texCoord;
}
