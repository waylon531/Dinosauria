#version 410

uniform sampler2D tex_color;

in vec2 vTexCoord;
layout (location=0) out vec4 fColor;

#define RADIUS 0.01
#define N_SAMPLES 7

const float SPACING=RADIUS/N_SAMPLES;

int gauss_fact[N_SAMPLES] = int[](1,6,15,20,15,6,1);

void main()
{
  fColor = vec4(0.0);
  float x=-RADIUS;
  int xx=0;
  //for(x=-RADIUS; x<=RADIUS; x+=SPACING)
  for(xx=0; xx<N_SAMPLES*2; xx++)
    {
      fColor += float(gauss_fact[xx/2]) * texture2D(tex_color, vTexCoord.st + vec2(0.0,x));
      x += SPACING;
    }
  fColor /= N_SAMPLES*2;
}
