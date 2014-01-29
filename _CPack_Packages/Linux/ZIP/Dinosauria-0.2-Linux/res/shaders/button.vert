#version 410

uniform mat4 m_ortho;

layout (location = 0) in vec2 position;

void main()
{
  gl_Position = m_ortho*vec4(position,0.0f,1.0f);
}
