#version 410

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_project;
uniform sampler2D tex_specular;
uniform sampler2D tex_normal;
uniform int waterFlag;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;

out vec4 fColor;
out vec4 fNormal;
out vec4 fPosition;
out vec4 fParams;

void main()
{
  if(waterFlag==1)
    {
      //culling
      if(vPosition.y < 0.f)
	{
	  discard;
	}
    }
  vec3 bitangent = normalize(cross(vNormal, vTangent));
  fColor = vec4(.7,.7,.7,1.);//texture2D(tex_specular, vTexCoord);
  vec3 nmap = mix(vec3(0,1,0),texture2D(tex_normal, vTexCoord).rgb,0.5);
  vec3 normal = normalize(nmap.x * vTangent + nmap.y * vNormal + nmap.z * bitangent);
  fNormal = vec4((m_model * vec4(normal,0.f)).xyz,1.f);
  fPosition = vec4(vPosition,1.f);
  fParams = vec4(1.,.5,5,1.f);
}
