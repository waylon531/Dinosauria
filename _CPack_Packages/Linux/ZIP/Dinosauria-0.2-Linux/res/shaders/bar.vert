#version 410

uniform mat4 m_ortho;

in vec2 position;

void main()
{
  gl_Position = (m_ortho * vec4(position,0.f,1.f));
}
