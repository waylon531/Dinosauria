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

in vec3 vNormal;

layout (location=0) out vec4 fColor;

layout (location=0) in vec2 gTexCoord;
layout (location=1) in vec3 gNormal;
layout (location=2) in vec3 gPosition;
layout (location=3) in vec3 gPatchDist;
layout (location=4) in vec4 gPositionSS;

float occluded(vec3 normal)
{
  //if(sunDir.y < 0.f) return 0.f;
  
  int b;
  vec4 coords;
  vec2 UV;
  float scale = 0.5;
  for(b=0; b<N_SHADOW_BUFFERS; b++)
    {
      vec4 lightSpace = m_light[b] * vec4(gPosition,1.f);
      coords = lightSpace / lightSpace.w;
      UV = coords.xy;
      if(!(UV.x < 0.0 || UV.x > 1.0 || UV.y < 0.0 || UV.y > 1.0)) break;
      scale *= 2.0;
    }
  //UV.x = 0.5 * coords.x + 0.5;
  //UV.y = 0.5 * coords.y + 0.5;
  //lightSpace /= lightSpace.w;
  float depth = coords.z;
  float bias = clamp(0.1*tan(acos(clamp(dot(sunDir,normal),0.0,1.0))),0.0,0.01);
  float depthS = texture2D(tex_shadow[b], UV).x;
  if(depthS < (depth - bias*scale)) return 0.0;
  return 1.0;
}

#define AMBIENT .2
#define DIFFUSE .6
#define SPECULAR 0.0
vec3 computeLighting(vec3 base, vec3 normal)
{
  float specular = clamp(dot(eyeDir,reflect(sunDir,normal)),0.0,1.0);
  specular = pow(specular,20.0);
  float diffuse = clamp(dot(normal,sunDir),0.0,1.0);
  float ambient = 1.0;
  return base*((SPECULAR*specular + DIFFUSE*diffuse)*occluded(normal) + AMBIENT*ambient);
}

float amplify(float d, float scale , float offset)
{
  d = scale * d + offset;
  d = clamp(d, 0, 1);
  d = 1- exp2(-2*d*d);
  return d;
}

void main()
{
  vec3 normal = texture2D(tex_normal, gTexCoord.st).rgb;
  vec3 dirt = texture2D(tex_dirt, gPosition.xz*.1f).rgb;
  vec3 rock = clamp(texture2D(tex_rock, gPosition.xz*.2f).rgb,0.0,1.0);
  //float l = abs(normal.x)+abs(normal.z)+abs(normal.y);
  float blend = texture2D(tex_blend,gTexCoord.st).r;
  vec3 base = mix(dirt,rock, clamp((texture2D(tex_height, gTexCoord.st).r+2.0)/10.0, 0.0,1.0));
  //vec3 base = mix(vec3(.25,.9,.25), vec3(.6,.6,.6), blend);
  //vec3 base = dirt;
  //vec3 normal = gNormal;
  fColor = vec4(computeLighting(base, normal), 1.0f);
  if(isWireframe==1)
    {
      float d = min(min(gPatchDist.x, gPatchDist.y), gPatchDist.z);
      float a = 1.0-amplify(d, 40, 0.0);
      fColor.rgb += vec3(step(0.5,a),0.f,0.f);
    }
  //fColor = vec4(texture2D(tex_height, eTexCoord.st).rgb,1.f);
  //fColor = vec4(normal,1.f);
}
