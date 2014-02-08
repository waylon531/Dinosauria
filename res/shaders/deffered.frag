#version 410

#define N_SHADOW_BUFFERS 5
#define M_PI 3.1415926535897932384626433832795

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
uniform int passNum;

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

float toShadowDepth(float z)
{
  float n = -40; // camera z near
  float f = 40.0; // camera z far
  return 2. * (2.0 * n) / (f + n - z * (f - n));
}

float rand(vec2 n)
{
  return fract(sin(dot(n.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float nrand(vec2 n)
{
  return rand(n)*2.0-1.0;
}

#define AO_RAD 0.01
#define AO_SAMPLES 10
#define AO_FALLOFF .000002
#define AO_STRENGTH 0.07
float getSSAO(vec3 normal)
{
  vec3 pSphere[10] = vec3[](vec3(-0.010735935, 0.01647018, 0.0062425877),vec3(-0.06533369, 0.3647007, -0.13746321),vec3(-0.6539235, -0.016726388, -0.53000957),vec3(0.40958285, 0.0052428036, -0.5591124),vec3(-0.1465366, 0.09899267, 0.15571679),vec3(-0.44122112, -0.5458797, 0.04912532),vec3(0.03755566, -0.10961345, -0.33040273),vec3(0.019100213, 0.29652783, 0.066237666),vec3(0.8765323, 0.011236004, 0.28265962),vec3(0.29264435, -0.40794238, 0.15964167));
  vec3 fres = normalize(vec3(nrand(vTexCoord.st), nrand(vTexCoord.st+vec2(1,1)), nrand(vec2(1,1)-vTexCoord.st)));
  float bl = 0.0;
  float radD = AO_RAD / depth;
  float occDepth, depthDiff;
  vec3 ray;
  vec3 occNorm;
  vec3 sNormal = (m_project * vec4(normal,0.f)).xyz;
  for(int i=0; i<AO_SAMPLES;++i)
    {
      ray = radD * reflect(pSphere[i], fres);
      vec2 coord = vTexCoord.st + /*sign(dot(ray,sNormal))**/ray.xy;
      occDepth = toDepth(texture2D(tex_depth, coord).r);
      occNorm = texture2D(tex_normal, coord).xyz;
      depthDiff = (depth - occDepth);
      if(depthDiff>0.0)
	{
	  bl += max(0.0,step(AO_FALLOFF, depthDiff) * (1.0-dot(occNorm,normal)) * (1.0-smoothstep(AO_FALLOFF,AO_STRENGTH,depthDiff)));
	}
    }
  return 1.0 - bl/2.0;
}

#define SHADOW_RES 1024
#define USE_PCF
#define USE_ESM
#define ESM_C 20.0

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
      scale *= 4.0;
      if(b==N_SHADOW_BUFFERS) return 1.0;
    }
  //if((UV.x < 0.0 || UV.x > 1.0 || UV.y < 0.0 || UV.y > 1.0)) return 1.0;
  //UV.x = 0.5 * coords.x + 0.5;
  //UV.y = 0.5 * coords.y + 0.5;
  //lightSpace /= lightSpace.w;
  float depth = coords.z;
  float bias = clamp(0.03*tan(acos(clamp(dot(sunDir,normal),0.0,1.0))),0.0,0.05);
  //bias = 0.005;
  ivec2 pix = ivec2(UV*SHADOW_RES);
  float xoff = (UV.x - float(pix.x)/SHADOW_RES)*SHADOW_RES;
  float yoff = (UV.y - float(pix.y)/SHADOW_RES)*SHADOW_RES;
  

#ifdef USE_ESM
#define SHADOWMAP(texCoord) clamp(2.0-exp(ESM_C*(depth - texture2D(tex_shadow[b],texCoord).x)), 0.0, 1.0)
#else
#define SHADOWMAP(texCoord) (texture2D(tex_shadow[b],texCoord).x < (depth-bias) ? 0.:1.)
#endif
  /*return SHADOWMAP(UV);
  float depthS = texture2D(tex_shadow[b], UV).x;
  if(depthS < (depth-bias)) return 0.;
  return 1.0;*/

#ifdef USE_PCF
  return mix(mix(SHADOWMAP(vec2(pix.x,pix.y)/SHADOW_RES), SHADOWMAP(vec2(pix.x+1,pix.y)/SHADOW_RES), xoff), mix(SHADOWMAP(vec2(pix.x,pix.y+1)/SHADOW_RES), SHADOWMAP(vec2(pix.x+1,pix.y+1)/SHADOW_RES), xoff), yoff);
#endif
#ifdef USE_ESM
  float depthS = texture2D(tex_shadow[b],UV).x;
  //if(depthS > depth-bias) return 1.0;
  return clamp(2.0-exp(ESM_C*(depth - depthS)), 0.0, 1.0);
#endif
  //return mix(mix(shadowmap(vec2(pix.x,pix.y)/SHADOW_RES, bias, b), shadowmap(vec2(pix.x+1,pix.y)/SHADOW_RES, bias, b), xoff), mix(shadowmap(vec2(pix.x,pix.y+1)/SHADOW_RES, bias, b), shadowmap(vec2(pix.x+1,pix.y+1)/SHADOW_RES, bias, b),xoff), yoff);;
}

float calculateDiffuse(vec3 normal)
{
  return params.x * clamp(dot(normal,sunDir),0.0,1.0);
}

float calculateSpecular(vec3 normal)
{ 
  float specular = clamp(dot(eyeDir,reflect(sunDir,normal)),0.0,1.0);
  return params.y*pow(specular,params.z);
}
  
#define AMBIENT .0
#define AO .2
vec3 computeLighting(vec3 base, vec3 normal)
{
  float ambient = 1.0;
  float ao = getSSAO(normal);
  //return vec3(ao)*AO;
  return base * ((calculateSpecular(normal) + calculateDiffuse(normal))*occluded(normal) + AMBIENT*ambient + AO*ao);
}
/*
//nitrogen
vec3 Kr = vec3(
	       0.18867780436772762, 0.4978442963618773, 0.6616065586417131
	       );

float phase(float alpha, float g)
{
  float a = 3.0*(1.0-g*g);
  float b = 2.0*(2.0+g*g);
  float c = 1.0+alpha*alpha;
  float d = pow(1.0+g*g-2.0*g*alpha, 1.5);
  return (1/b)*(c/d);
}

float atmospheric_depth(vec3 position, vec3 dir)
{
  float a = dot(dir, dir);
  float b = 2.0*dot(dir, position);
  float c = dot(position, position)-1.0;
  float det = b*b-4.0*a*c;
  float detSqrt = sqrt(det);
  float q = (-b - detSqrt)/2.0;
  float t1 = c/q;
  return t1;
}

float horizon_extinction(vec3 position, vec3 dir, float radius)
{
  float u = dot(dir, -position);
  if(u<0.0){
    return 1.0;
  }
  vec3 near = position + u*dir;
  if(length(near) < radius){
    return 0.0;
  }
  else{
    vec3 v2 = normalize(near)*radius - position;
    float diff = acos(dot(normalize(v2), dir));
    return smoothstep(0.0, 1.0, pow(diff*2.0, 3.0));
  }
}

vec3 absorb(float dist, vec3 color, float factor)
{
  return color-color*pow(Kr, vec3(factor/dist));
}


#define SUN_RADIUS .01
#define SUN_ENERGY 15.
#define RAYLEIGH_SCALE 33.3
#define MIE_SCALE .1
#define SPOT_SCALE 1000.0
#define STEP_COUNT 16
#define SCATTER_STRENGTH .25//.025
#define RAYLEIGH_STRENGTH 1.39
#define MIE_STRENGTH .0264
#define MIE_POW .39
#define RAYLEIGH_POW .81
#define MIE_DISTRIB 6.3
#define INTENSITY 1.8
#define PLAYER_HEIGHT 0.99
#define H_BIAS 0.15

vec3 calcSkyColor()
{
  //unproject onto sphere
  vec3 rayDir = (inverse(m_view) * vec4((inverse(m_project)*vec4(vTexCoord.st*2.0-1.0,0.f,1.f)).xyz,0.f)).xyz;
  vec3 skyPos = normalize(rayDir);
  //check if sun should be drawn
  //return vec3(dot(sunDir,skyPos));
  //if(dot(sunDir, skyPos) > cos(SUN_RADIUS))
  //  {
  //    return vec3(SUN_ENERGY);
  //  }
  //get sky color
  float alpha = dot(sunDir, skyPos);

  vec3 playerPos = vec3(0,PLAYER_HEIGHT,0);

  float rayleigh_factor = phase(alpha, -0.01) * RAYLEIGH_SCALE;
  float mie_factor = phase(alpha, MIE_DISTRIB) * MIE_SCALE;
  float spot = smoothstep(0.0, SUN_ENERGY, 0.01*phase(alpha, 0.99995)) * SPOT_SCALE;
  float eyeDepth = atmospheric_depth(playerPos,skyPos);
  float eyeExtinction = horizon_extinction(playerPos, skyPos, PLAYER_HEIGHT-H_BIAS);

  
  float stepLen = eyeDepth/float(STEP_COUNT);
  vec3 rayleigh = vec3(0);
  vec3 mie = vec3(0);
  for(int i=0; i<STEP_COUNT; i++)
    {
      float sampleDist = playerPos + stepLen * float(i);
      vec3 pos = skyPos*sampleDist;
      float sampleDepth = atmospheric_depth(pos, skyPos);
      float sampleExtinction = horizon_extinction(pos, skyPos, PLAYER_HEIGHT - H_BIAS);
      
      vec3 influx = absorb(sampleDepth, vec3(INTENSITY), SCATTER_STRENGTH) * sampleExtinction;
      rayleigh += absorb(sampleDist, Kr*influx, RAYLEIGH_SCALE);
      mie += absorb(sampleDist, influx, MIE_SCALE);
    }
  rayleigh = (rayleigh * eyeExtinction * pow(eyeDepth, RAYLEIGH_POW))/float(STEP_COUNT);
  mie = (mie * pow(eyeDepth, MIE_POW))/float(STEP_COUNT);
  //return vec3(eyeDepth);
  return spot*mie + mie_factor*mie + rayleigh_factor*rayleigh;
}
*/

//sky code borrwed from http://codeflow.org/entries/2011/apr/13/advanced-webgl-part-2-sky-rendering/
vec3 Kr = vec3(0.18867780436772762, 0.4978442963618773, 0.6616065586417131); // air
//uniform float rayleigh_brightness, mie_brightness, spot_brightness, scatter_strength, rayleigh_strength, mie_strength;
//uniform float rayleigh_collection_power, mie_collection_power, mie_distribution;
#define rayleigh_brightness 3.3
#define mie_brightness .1
#define spot_brightness 1000.0
#define scatter_strength .028
#define rayleigh_strength .139
#define mie_strength .0264
#define rayleigh_collection_power .81
#define mie_collection_power .39
#define mie_distribution .63

float surface_height = 0.99;
float range = 0.01;
float intensity = 1.8;
const int step_count = 16;

float atmospheric_depth(vec3 position, vec3 dir){
  float a = dot(dir, dir);
  float b = 2.0*dot(dir, position);
  float c = dot(position, position)-1.0;
  float det = b*b-4.0*a*c;
  float detSqrt = sqrt(det);
  float q = (-b - detSqrt)/2.0;
  float t1 = c/q;
  return t1;
}

float phase(float alpha, float g){
  float a = 3.0*(1.0-g*g);
  float b = 2.0*(2.0+g*g);
  float c = 1.0+alpha*alpha;
  float d = pow(1.0+g*g-2.0*g*alpha, 1.5);
  return (a/b)*(c/d);
}

float horizon_extinction(vec3 position, vec3 dir, float radius){
  float u = dot(dir, -position);
  if(u<0.0){
    return 1.0;
  }
  vec3 near = position + u*dir;
  if(length(near) < radius){
    return 0.0;
  }
  else{
    vec3 v2 = normalize(near)*radius - position;
    float diff = acos(dot(normalize(v2), dir));
    return smoothstep(0.0, 1.0, pow(diff*2.0, 3.0));
  }
}

vec3 absorb(float dist, vec3 color, float factor){
  return color-color*pow(Kr, vec3(factor/dist));
}

vec3 calcSkyColor()
{
  vec3 lightdir = sunDir;
  vec3 rayDir = (inverse(m_view) * vec4((inverse(m_project)*vec4(vTexCoord.st*2.0-1.0,0.f,1.f)).xyz,0.f)).xyz;
  vec3 eyedir = normalize(rayDir);
  float alpha = dot(eyedir, lightdir);

  float rayleigh_factor = phase(alpha, -0.01)*rayleigh_brightness;
  float mie_factor = phase(alpha, mie_distribution)*mie_brightness;
  float spot = smoothstep(0.0, 15.0, phase(alpha, 0.9995))*spot_brightness;

  vec3 eye_position = vec3(0.0, surface_height, 0.0);
  float eye_depth = atmospheric_depth(eye_position, eyedir);
  float step_length = eye_depth/float(step_count);
  float eye_extinction = horizon_extinction(eye_position, eyedir, surface_height-0.15);

  vec3 rayleigh_collected = vec3(0.0, 0.0, 0.0);
  vec3 mie_collected = vec3(0.0, 0.0, 0.0);

  for(int i=0; i<step_count; i++){
    float sample_distance = step_length*float(i);
    vec3 position = eye_position + eyedir*sample_distance;
    float extinction = horizon_extinction(position, lightdir, surface_height-0.35);
    float sample_depth = atmospheric_depth(position, lightdir);
    vec3 influx = absorb(sample_depth, vec3(intensity), scatter_strength)*extinction;
    rayleigh_collected += absorb(sample_distance, Kr*influx, rayleigh_strength);
    mie_collected += absorb(sample_distance, influx, mie_strength);
  }

  rayleigh_collected = (rayleigh_collected*eye_extinction*pow(eye_depth, rayleigh_collection_power))/float(step_count);
  mie_collected = (mie_collected*eye_extinction*pow(eye_depth, mie_collection_power))/float(step_count);

  vec3 color = vec3(spot*mie_collected + mie_factor*mie_collected + rayleigh_factor*rayleigh_collected);

  return color;
}

void main()
{
  vec3 normal = texture2D(tex_normal, vTexCoord.st).xyz;
  vec3 color = texture2D(tex_color, vTexCoord.st).rgb;
  params = texture2D(tex_params, vTexCoord.st).xyz;
  position = texture2D(tex_position, vTexCoord.st).xyz;
  float z = texture2D(tex_depth, vTexCoord.st).r;
  depth = toDepth(z);
  if(1.0 == z)
    {
      fColor = vec4(calcSkyColor(), 1.f);
      return;
    }
  if(passNum==0)
    {
      fColor = vec4(computeLighting(color, normal),1.f);
    }
  if(passNum==1)
    {
      fColor = vec4(normal, 1.f);
    }
  if(passNum==2)
    {
      fColor = vec4(vec3(z),1.f);
    }
  if(passNum==3)
    {
      fColor = vec4(vec3(getSSAO(normal)),1.f);
    }
  if(passNum==4)
    {
      fColor = vec4(color,1.f);
    }
  if(passNum==5)
    {
      fColor = vec4(vec3(calculateDiffuse(normal)),1.f);
    }
  if(passNum==6)
    {
      fColor = vec4(vec3(calculateSpecular(normal)),1.f);
    }
  if(passNum==7)
    {
      fColor = vec4(computeLighting(vec3(1,1,1),normal),1.f);
    }
  if(passNum==8)
    {
      fColor = vec4(vec3(occluded(normal)),1.f);
    }
}
