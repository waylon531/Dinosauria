#version 410

#define N_SHADOW_BUFFERS 4
uniform sampler2D tex_height;
uniform sampler2D tex_normal;
uniform sampler2D tex_blend;
uniform sampler2D tex_dirt;
uniform sampler2D tex_rock;
uniform sampler2D tex_shadow[N_SHADOW_BUFFERS];
uniform vec3 eyeDir;
uniform vec3 sunDir;
uniform unsigned int isWireframe;
uniform mat4 m_light[N_SHADOW_BUFFERS];
uniform int waterFlag;

in vec3 vNormal;

layout (location=0) out vec4 fColor;
out vec4 fNormal;
out vec4 fPosition;
out vec4 fParams;

layout (location=0) in vec2 gTexCoord;
layout (location=1) in vec3 gNormal;
layout (location=2) in vec3 gPosition;
layout (location=3) in vec3 gPatchDist;
layout (location=4) in vec4 gPositionSS;

float amplify(float d, float scale , float offset)
{
  d = scale * d + offset;
  d = clamp(d, 0, 1);
  d = 1- exp2(-2*d*d);
  return d;
}

void main()
{
  if(waterFlag==1)
    {
      //culling
      if(gPosition.y < 0.0f)
	{
	  discard;
	}
    }
      
  vec3 normal = texture2D(tex_normal, gTexCoord.st).rgb;
  fNormal = vec4(normal,1.f);
  vec3 dirt = texture2D(tex_dirt, gPosition.xz*.1f).rgb;
  vec3 rock = clamp(texture2D(tex_rock, gPosition.xz*.2f).rgb,0.0,1.0);
  //float l = abs(normal.x)+abs(normal.z)+abs(normal.y);
  float blend = texture2D(tex_blend,gTexCoord.st).r;
  vec3 base = mix(dirt,rock, clamp((texture2D(tex_height, gTexCoord.st).r+2.0)/10.0, 0.0,1.0));
  //vec3 base = mix(vec3(.25,.9,.25), vec3(.6,.6,.6), blend);
  //vec3 base = dirt;
  //vec3 normal = gNormal;
  fColor = vec4(base,1.f);
  //fColor = vec4(computeLighting(base, normal), 1.0f);
  if(isWireframe==1)
    {
      float d = min(min(gPatchDist.x, gPatchDist.y), gPatchDist.z);
      float a = 1.0-amplify(d, 40, 0.0);
      fColor.rgb += vec3(step(0.5,a),0.f,0.f);
    }
  fParams = vec4(.6,.0,1.,1.);
  fPosition = vec4(gPosition,1.f);
  //fColor = vec4(texture2D(tex_height, eTexCoord.st).rgb,1.f);
  //fColor = vec4(normal,1.f);
}
