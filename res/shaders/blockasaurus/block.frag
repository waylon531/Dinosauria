#version 410
#define N_SHADOW_BUFFERS 4

uniform int t;
uniform sampler2D tex_skin;
uniform sampler2D tex_shadow[N_SHADOW_BUFFERS];
uniform vec3 eyeDir;
uniform vec3 sunDir;
uniform mat4 m_light[N_SHADOW_BUFFERS];

layout (location=0) in vec3 vNormal;
layout (location=1) in vec2 vTexCoord;
layout (location=2) in vec4 vPosition;
layout (location=3) in vec4 vPositionView;

layout (location=0) out vec4 fColor;
layout (location=1) out vec4 fNormal;
layout (location=2) out vec4 fPosition;
layout (location=4) out vec4 fParams;


float occluded(vec3 normal)
{
  //if(sunDir.y < 0.f) return 0.f;
  
  int b;
  vec4 coords;
  vec2 UV;
  float scale = 0.5;
  for(b=0; b<N_SHADOW_BUFFERS; b++)
    {
      vec4 lightSpace = m_light[b] * vec4(vPosition.xyz,1.f);
      coords = lightSpace / lightSpace.w;
      UV = coords.xy;
      if(!(UV.x < 0.0 || UV.x > 1.0 || UV.y < 0.0 || UV.y > 1.0)) break;
      scale *= 2.0;
    }
  //UV.x = 0.5 * coords.x + 0.5;
  //UV.y = 0.5 * coords.y + 0.5;
  //lightSpace /= lightSpace.w;
  float depth = coords.z;;
  float bias = clamp(0.05*tan(acos(clamp(dot(sunDir,normal),0.0,1.0))),0.0,0.01);
  float depthS = texture2D(tex_shadow[b], UV).x;
  if(depthS < (depth - bias*scale)) return 0.0;
  return 1.0;
}

#define AMBIENT .2
#define DIFFUSE .7
#define SPECULAR 2.
vec3 computeLighting(vec3 base, vec3 normal)
{
  float specular = clamp(dot(eyeDir,reflect(sunDir,normal)),0.0,1.0);
  specular = pow(specular,15.0);
  float diffuse = clamp(dot(normal,sunDir),0.0,1.0);
  float ambient = 1.0;
  return base*((SPECULAR*specular + DIFFUSE*diffuse)*occluded(normal) + AMBIENT*ambient);
}

void main()
{
  vec3 base = texture2D(tex_skin, vTexCoord.st/2.f).xyz;
  //fColor = vec4(computeLighting(base, normalize(vNormal)), 1.0f);
  fColor = vec4(base,1.f);
  fNormal = vec4(normalize(vNormal),1.f);
  fParams = vec4(.7,2.,15.,1.);
  fPosition = vPosition;
}
