#version 410

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_project;
uniform mat4 boneTransforms[32];

in vec3 position;
in vec3 normal;
in vec2 texCoord;
in vec3 tangent;

out vec3 vNormal;
out vec3 vPosition;
out vec2 vTexCoord;
out vec3 vTangent;

void main()
{
  vec3 position2 = position;
  gl_Position = (m_project * m_view * m_model) * vec4(position2,1.f);
  vNormal = normal;
  vTangent = tangent;
  vPosition = (m_model * vec4(position2,1.f)).xyz;
  vTexCoord = texCoord;
}
