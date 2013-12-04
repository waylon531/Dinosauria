#version 410

uniform sampler2D tex_in;
uniform sampler2D tex_depth;
uniform float t;

layout (location = 0) in vec2 vTexCoord;

out vec4 fColor;

void main()
{
  vec2 texCoord = vTexCoord;
  float depth = texture2D(tex_depth, texCoord.st)*100.0;
  //float invDepth = 1.0/depth;
  //float fog = (invDepth*invDepth)*4000.0;
#define DENSITY 0.0001
  float fog = depth*depth*DENSITY;
  fog = fog;
  //texCoord.x += depth*sin(texCoord.y*20.0) * sin(t/20.0)/20.0;
  
  fColor = mix(vec4(texture2D(tex_in, texCoord.st).xyz, 1.0f), vec4(0.0f,0.0f,0.0f,1.0f) , fog);
}
