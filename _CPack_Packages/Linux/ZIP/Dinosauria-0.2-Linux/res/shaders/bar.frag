#version 410

uniform vec4 color;

out vec4 fColor;

void main()
{
  fColor = color;
  //fColor = vec4(color.rgb,1.f);
  //fColor = vec4(1.f,0.f,0.f,1.f);
}
