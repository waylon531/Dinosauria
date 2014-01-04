#version 410

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_project;
uniform sampler2D tex_height;
uniform mat4 m_tessView;

layout (location=0) in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec2 vTexCoord;
out vec3 vPositionWorld;
out vec3 vPositionEye;
out vec3 vPositionCam;
out vec4 absPosition;

void main()
{
  vTexCoord = texCoord;
  vec3 positionh = vec3(position.x, texture2D(tex_height,texCoord.st).r, position.z);
  mat4 PVM = m_project*m_view*m_model;
  gl_Position = PVM*vec4(positionh,1.f);
  vPositionWorld = (m_model * vec4(positionh,1.f)).xyz;
  vec4 tmp = ((m_project*m_tessView*m_model) * vec4(positionh,1.f));
  vPositionEye = tmp.xyz/tmp.w;
  vPositionCam = ((m_tessView*m_model) * vec4(positionh,1.0)).xyz;
  absPosition = vec4(position,1.0);
}
