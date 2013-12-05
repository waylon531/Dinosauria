#version 410

uniform sampler2D tex_color;

in vec2 vTexCoord;
layout (location=0) out vec4 fColor;

#define RADIUS 0.005
#define N_SAMPLES 8

const float SPACING=RADIUS/N_SAMPLES;

void main()
{
  fColor = vec4(0.0);
  float x;
  for(x=-RADIUS; x<=RADIUS; x+=SPACING)
    {
      fColor += texture2D(tex_color, vTexCoord.st + vec2(x,0.0));
    }
  fColor /= N_SAMPLES;
}
