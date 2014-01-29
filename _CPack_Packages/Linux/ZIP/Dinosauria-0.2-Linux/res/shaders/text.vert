#version 410

uniform mat4 m_ortho;

layout (location=0) in vec2 position;
layout (location=1) in vec2 texCoord;
layout (location=2) in vec4 color;

out vec3 vPosition;
out vec2 vTexCoord;
out vec4 vColor;

void main()
{
  vPosition = vec3(position,0.f);
  gl_Position = m_ortho * vec4(vPosition,1.0f);
  vTexCoord = texCoord;
  vColor = color;
}
