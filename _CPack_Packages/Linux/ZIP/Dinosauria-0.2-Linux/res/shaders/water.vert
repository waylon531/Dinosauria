#version 410

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_project;

in vec3 position;
in vec2 texCoord;

out vec2 vTexCoord;

void main()
{
  vTexCoord = texCoord;
  gl_Position = m_project*m_view*m_model * vec4(position,1.0);
}
