#version 410

uniform sampler2D tex_color;

in vec2 vTexCoord;

out vec4 fColor;

#define THRESHOLD sqrt(2.f)

vec3 thresh(vec3 x)
{
  return x*step(THRESHOLD,(x.r+x.g+x.b)/3.f);
}

vec3 diffraction(float scale)
{
  return vec3(abs(sin(scale)), abs(sin(scale*3)), abs(sin(scale*7)));
}

#define N_SAMPLES 8
#define BLUR_RADIUS 0.02
vec3 samplePt(vec2 texCoord)
{
  if(texCoord.s < 0.0 || texCoord.s > 1.0 || texCoord.t < 0.0 || texCoord.s > 1.0)
    {
      return vec3(0,0,0);
    }
  //thresh(texture2D(tex_color, vec2(texCoord.s )).rgb);
  //preform blut
  vec3 final;
  for(int i=0; i<N_SAMPLES; i++)
    {
      final += thresh(texture2D(tex_color, vec2(texCoord.s + BLUR_RADIUS*(i/float(N_SAMPLES) - 0.5)*2.0, texCoord.t)).rgb);
    }
  final /= float(N_SAMPLES);
  return final;
}

vec3 getVal(float scale)
{
  vec2 texCoord = vTexCoord*2.0-1.0;
  return 0.03*diffraction(scale)*samplePt((texCoord.st*scale + 1.0)*0.5);
  return vec3(0,0,0);
}

void main()
{
  fColor =  texture2D(tex_color, vTexCoord.st) +
    vec4(getVal(1.5) + getVal(-1.5) + getVal(1.75) + getVal(-1.75) + getVal(2) + getVal(-2) + getVal(3) + getVal(-3), 0.f);
}
