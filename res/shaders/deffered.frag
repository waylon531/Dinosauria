#version 410

#define N_SHADOW_BUFFERS 4

uniform sampler2D tex_color;
uniform sampler2D tex_normal;
uniform sampler2D tex_params;
uniform sampler2D tex_depth;
uniform sampler2D tex_position;
uniform mat4 m_view;
uniform mat4 m_project;
uniform vec3 sunDir,eyeDir;
uniform sampler2D tex_shadow[N_SHADOW_BUFFERS];
uniform mat4 m_light[N_SHADOW_BUFFERS];

in vec2 vTexCoord;
out vec4 fColor;

vec3 position;
vec3 params;
float depth;

float toDepth(float z)
{
  float n = .5; // camera z near
  float f = 200.0; // camera z far
  return 2. * (2.0 * n) / (f + n - z * (f - n));
}

float rand(vec2 n)
{
  return fract(sin(dot(n.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

#define AO_RAD 0.006
#define AO_SAMPLES 10
#define AO_FALLOFF .000002
#define AO_STRENGTH 0.07
float getSSAO(vec3 normal)
{
  vec3 pSphere[10] = vec3[](vec3(-0.010735935, 0.01647018, 0.0062425877),vec3(-0.06533369, 0.3647007, -0.13746321),vec3(-0.6539235, -0.016726388, -0.53000957),vec3(0.40958285, 0.0052428036, -0.5591124),vec3(-0.1465366, 0.09899267, 0.15571679),vec3(-0.44122112, -0.5458797, 0.04912532),vec3(0.03755566, -0.10961345, -0.33040273),vec3(0.019100213, 0.29652783, 0.066237666),vec3(0.8765323, 0.011236004, 0.28265962),vec3(0.29264435, -0.40794238, 0.15964167));
  vec3 fres = normalize(vec3(rand(vTexCoord.st), rand(vTexCoord.ts), rand(1.0-vTexCoord.st))*2.0-1.0);
  float bl = 0.0;
  float radD = AO_RAD / depth;
  float occDepth, depthDiff;
  vec3 ray;
  vec3 occNorm;
  for(int i=0; i<AO_SAMPLES;++i)
    {
      ray = radD * reflect(pSphere[i], fres);
      vec2 coord = vTexCoord.st + sign(dot(ray,normal))*ray.xy;
      occDepth = toDepth(texture2D(tex_depth, coord).r);
      occNorm = texture2D(tex_normal, coord).xyz;
      depthDiff = depth - occDepth;
      if(depthDiff>0.0)
	{
	  bl += max(0.0,step(AO_FALLOFF, depthDiff) /** (1.0-dot(occNorm,normal)) */* (1.0-smoothstep(AO_FALLOFF,AO_STRENGTH,depthDiff)));
	}
    }
  return 1.0 - bl/10.0;
}


float occluded(vec3 normal)
{
  //if(sunDir.y < 0.f) return 0.f;
  
  int b;
  vec4 coords;
  vec2 UV;
  float scale = 0.5;
  for(b=0; b<N_SHADOW_BUFFERS; b++)
    {
      vec4 lightSpace = m_light[b] * vec4(position,1.f);
      coords = lightSpace / lightSpace.w;
      UV = coords.xy;
      if(!(UV.x < 0.0 || UV.x > 1.0 || UV.y < 0.0 || UV.y > 1.0)) break;
      scale *= 2.0;
    }
  if((UV.x < 0.0 || UV.x > 1.0 || UV.y < 0.0 || UV.y > 1.0)) return 1.0;
  //UV.x = 0.5 * coords.x + 0.5;
  //UV.y = 0.5 * coords.y + 0.5;
  //lightSpace /= lightSpace.w;
  float depth = coords.z;
  float bias = clamp(0.01*tan(acos(clamp(dot(sunDir,normal),0.0,1.0))),0.0,0.01);
  float depthS = texture2D(tex_shadow[b], UV).x;
  if(depthS < (depth - bias)) return 0.0;
  return 1.0;
}

#define AMBIENT .0
#define AO .3
vec3 computeLighting(vec3 base, vec3 normal)
{
  float specular = clamp(dot(eyeDir,reflect(sunDir,normal)),0.0,1.0);
  specular = pow(specular,params.z);
  float diffuse = clamp(dot(normal,sunDir),0.0,1.0);
  float ambient = 1.0;
  float ao = getSSAO(normal);
  //return vec3(ao)*AO;
  return base*((params.y*specular + params.x*diffuse)*occluded(normal) + AMBIENT*ambient + AO*ao);
}


void main()
{
  vec3 normal = texture2D(tex_normal, vTexCoord.st).xyz;
  vec3 color = texture2D(tex_color, vTexCoord.st).rgb;
  params = texture2D(tex_params, vTexCoord.st).xyz;
  position = texture2D(tex_position, vTexCoord.st).xyz;
  depth = toDepth(texture2D(tex_depth, vTexCoord.st).r);
  fColor = vec4(computeLighting(color, normal),1.f);
  //fColor = vec4(params, 1.f);
}
