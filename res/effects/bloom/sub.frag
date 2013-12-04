#version 410

uniform sampler2D tex_color;

in vec2 vTexCoord;
out vec4 fColor;

#define VALUE 1.0

void main()
{
  vec4 color = texture2D(tex_color,vTexCoord.st);
  //fColor = clamp(color-vec4(vec3(VALUE),0.0),vec4(0.0),vec4(1.0))+vec4(vec3(VALUE),0.0);
  if(max(max(color.r,color.g),color.b) < 0.5)
    {
      fColor = vec4(0.0,0.0,0.0,1.0);
    }
  else
    {
      fColor = color;
    }
}
