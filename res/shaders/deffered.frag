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
uniform int time;

in vec2 vTexCoord;
out vec4 fColor;

//
// GLSL textureless classic 3D noise "cnoise",
// with an RSL-style periodic variant "pnoise".
// Author:  Stefan Gustavson (stefan.gustavson@liu.se)
// Version: 2011-10-11
//
// Many thanks to Ian McEwan of Ashima Arts for the
// ideas for permutation and gradient selection.
//
// Copyright (c) 2011 Stefan Gustavson. All rights reserved.
// Distributed under the MIT license. See LICENSE file.
// https://github.com/ashima/webgl-noise
//

vec3 mod289(vec3 x)
{
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x)
{
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x)
{
  return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

vec3 fade(vec3 t) {
  return t*t*t*(t*(t*6.0-15.0)+10.0);
}

// Classic Perlin noise
float cnoise(vec3 P)
{
  vec3 Pi0 = floor(P); // Integer part for indexing
  vec3 Pi1 = Pi0 + vec3(1.0); // Integer part + 1
  Pi0 = mod289(Pi0);
  Pi1 = mod289(Pi1);
  vec3 Pf0 = fract(P); // Fractional part for interpolation
  vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
  vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
  vec4 iy = vec4(Pi0.yy, Pi1.yy);
  vec4 iz0 = Pi0.zzzz;
  vec4 iz1 = Pi1.zzzz;

  vec4 ixy = permute(permute(ix) + iy);
  vec4 ixy0 = permute(ixy + iz0);
  vec4 ixy1 = permute(ixy + iz1);

  vec4 gx0 = ixy0 * (1.0 / 7.0);
  vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
  gx0 = fract(gx0);
  vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
  vec4 sz0 = step(gz0, vec4(0.0));
  gx0 -= sz0 * (step(0.0, gx0) - 0.5);
  gy0 -= sz0 * (step(0.0, gy0) - 0.5);

  vec4 gx1 = ixy1 * (1.0 / 7.0);
  vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
  gx1 = fract(gx1);
  vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
  vec4 sz1 = step(gz1, vec4(0.0));
  gx1 -= sz1 * (step(0.0, gx1) - 0.5);
  gy1 -= sz1 * (step(0.0, gy1) - 0.5);

  vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
  vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
  vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
  vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
  vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
  vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
  vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
  vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);

  vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
  g000 *= norm0.x;
  g010 *= norm0.y;
  g100 *= norm0.z;
  g110 *= norm0.w;
  vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
  g001 *= norm1.x;
  g011 *= norm1.y;
  g101 *= norm1.z;
  g111 *= norm1.w;

  float n000 = dot(g000, Pf0);
  float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
  float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
  float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
  float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
  float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
  float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
  float n111 = dot(g111, Pf1);

  vec3 fade_xyz = fade(Pf0);
  vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
  vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
  float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x);
  return 2.2 * n_xyz;
}

// Classic Perlin noise, periodic variant
float pnoise(vec3 P, vec3 rep)
{
  vec3 Pi0 = mod(floor(P), rep); // Integer part, modulo period
  vec3 Pi1 = mod(Pi0 + vec3(1.0), rep); // Integer part + 1, mod period
  Pi0 = mod289(Pi0);
  Pi1 = mod289(Pi1);
  vec3 Pf0 = fract(P); // Fractional part for interpolation
  vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
  vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
  vec4 iy = vec4(Pi0.yy, Pi1.yy);
  vec4 iz0 = Pi0.zzzz;
  vec4 iz1 = Pi1.zzzz;

  vec4 ixy = permute(permute(ix) + iy);
  vec4 ixy0 = permute(ixy + iz0);
  vec4 ixy1 = permute(ixy + iz1);

  vec4 gx0 = ixy0 * (1.0 / 7.0);
  vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
  gx0 = fract(gx0);
  vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
  vec4 sz0 = step(gz0, vec4(0.0));
  gx0 -= sz0 * (step(0.0, gx0) - 0.5);
  gy0 -= sz0 * (step(0.0, gy0) - 0.5);

  vec4 gx1 = ixy1 * (1.0 / 7.0);
  vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
  gx1 = fract(gx1);
  vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
  vec4 sz1 = step(gz1, vec4(0.0));
  gx1 -= sz1 * (step(0.0, gx1) - 0.5);
  gy1 -= sz1 * (step(0.0, gy1) - 0.5);

  vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
  vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
  vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
  vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
  vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
  vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
  vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
  vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);

  vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
  g000 *= norm0.x;
  g010 *= norm0.y;
  g100 *= norm0.z;
  g110 *= norm0.w;
  vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
  g001 *= norm1.x;
  g011 *= norm1.y;
  g101 *= norm1.z;
  g111 *= norm1.w;

  float n000 = dot(g000, Pf0);
  float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
  float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
  float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
  float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
  float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
  float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
  float n111 = dot(g111, Pf1);

  vec3 fade_xyz = fade(Pf0);
  vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
  vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
  float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x);
  return 2.2 * n_xyz;
}

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
#define ESM_C 80.0

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
  if(depth < 0.0 || depth > 1.0) return 1.0;
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

#define PERS 0.6
float cloudFractalNoise(vec3 v)
{
  //return cnoise(v) + (.5*cnoise(v*2)) + (.25*cnoise(v*4)) + (.125*cnoise(v*8)) + (.0625*cnoise(v*16));
  return cnoise(v) + (PERS*cnoise(v*2)) + (PERS*PERS*cnoise(v*4)) + (PERS*PERS*PERS*cnoise(v*8)) + (PERS*PERS*PERS*PERS*cnoise(v*16));
}

#define CLOUD_HEIGHT 10000.0
vec4 getCloudColor(vec3 dir)
{
  //get the point on a cloud plane
  vec3 point = dir * (CLOUD_HEIGHT / dir.y);
  if(dir.y<0.f) return vec4(0,0,0,0);
  vec3 windDisplace = vec3(0,float(time)*0.0025,0);
  float alpha = cloudFractalNoise(vec3(.00005*point.xz, float(time)*0.001) + windDisplace);
  return vec4(1.,1.,1., step(0.0,alpha)*alpha);
}

//sky code borrwed from http://codeflow.org/entries/2011/apr/13/advanced-webgl-part-2-sky-rendering/
vec3 Kr = vec3(0.18867780436772762, 0.4978442963618773, 0.6616065586417131); // air
//uniform float rayleigh_brightness, mie_brightness, spot_brightness, scatter_strength, rayleigh_strength, mie_strength;
//uniform float rayleigh_collection_power, mie_collection_power, mie_distribution;
#define rayleigh_brightness 3.3
#define mie_brightness .03
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
  float spot = smoothstep(0.0, 15.0, 0.001*phase(alpha, 0.9995))*spot_brightness;

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
  vec4 cloud = getCloudColor(eyedir);
  
  return mix(color, cloud.rgb, cloud.a);
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
