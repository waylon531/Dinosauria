#version 410
#define N_SHADOW_BUFFERS 4

uniform int t;
uniform sampler2D tex_skin;
uniform sampler2D tex_shadow[N_SHADOW_BUFFERS];
uniform vec3 eyeDir;
uniform vec3 sunDir;
uniform mat4 m_light[N_SHADOW_BUFFERS];
uniform int waterFlag;

layout (location=0) in vec3 vNormal;
layout (location=1) in vec2 vTexCoord;
layout (location=2) in vec4 vPosition;
layout (location=3) in vec4 vPositionView;

layout (location=0) out vec4 fColor;
layout (location=1) out vec4 fNormal;
layout (location=2) out vec4 fPosition;
layout (location=3) out vec4 fParams;

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
  vec3 base = texture2D(tex_skin, vTexCoord.st/2.f).xyz;
  //fColor = vec4(computeLighting(base, normalize(vNormal)), 1.0f);
  fColor = vec4(base,1.f);
  fNormal = vec4(normalize(vNormal),1.f);
  fParams = vec4(.7,2.,10.,1.);
  fPosition = vPosition;
}
