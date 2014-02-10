#version 410

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_project;
uniform mat4 boneTransforms[32];

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 texCoord;
layout (location=3) in ivec4 boneIds;
layout (location=4) in vec4 boneWeights;

out vec3 vNormal;
out vec2 vTexCoord;
out vec4 vPosition;
out vec4 vPositionView;

void main()
{
  mat4 m_model2 = mat4(1,0,0,0,
		       0,1,0,0,
		       0,0,1,0,
		       0,0,0,1);
  vec4 boneWeights2 = normalize(boneWeights);
  //if(boneWeights[0]!=0.f) m_model2 += (boneTransforms[boneIds[0]] * boneWeights2[0]);
  //if(boneWeights[1]!=0.f) m_model2 += (boneTransforms[boneIds[1]] * boneWeights2[1]);
  //if(boneWeights[2]!=0.f) m_model2 += (boneTransforms[boneIds[2]] * boneWeights2[2]);
  //if(boneWeights[3]!=0.f) m_model2 += (boneTransforms[boneIds[3]] * boneWeights2[3]);
  vec3 position2 = (m_model * m_model2 * vec4(position,1.f)).xyz;
  mat4 PVM = m_project*m_view;
  gl_Position = PVM*vec4(position2,1.f);
  vPosition = vec4(position2,1.f);
  vPositionView = gl_Position;
  vNormal = (m_model * m_model2 * vec4(normal,0.f)).xyz;
  vTexCoord = texCoord;
}
