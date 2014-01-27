#version 410

uniform mat4 m_project;
uniform mat4 m_view;
uniform mat4 m_model;
uniform mat4 boneTransforms[32];

layout (location=0) in vec3 position;
layout (location=3) in ivec4 boneIds;
layout (location=4) in vec4 boneWeights;

void main()
{
  mat4 m_model2 = m_model;
  if(boneWeights.x!=0.f) m_model2 += boneWeights.x * boneTransforms[boneIds.x];
  if(boneWeights.y!=0.f) m_model2 += boneWeights.y * boneTransforms[boneIds.y];
  if(boneWeights.z!=0.f) m_model2 += boneWeights.z * boneTransforms[boneIds.z];
  if(boneWeights.w!=0.f) m_model2 += boneWeights.w * boneTransforms[boneIds.w];
    
  gl_Position = m_project * m_view * m_model2 * vec4(position,1.f);
}
