#version 410
//the standard shader

uniform float t;

layout (location = 0) in vec3 vNormal;
layout (location = 1) in vec2 vTexCoord;

out vec4 fColor;

#define SPECULAR 0.5
#define DIFFUSE 0.5
#define AMBIENT 0.2
vec3 computeLighting(vec3 base, vec3 normal, vec3 lightDir)
{
  vec3 eyeDir = normalize(vec3(0.0,0.0,1.0));
  float specular = clamp(dot(eyeDir,reflect(-lightDir,normal)),0.0,1.0);
  specular = pow(specular,10.0);
  float diffuse = clamp(dot(normal,lightDir),0.0,1.0);
  float ambient = 1.0;
  return base*(specular*SPECULAR + diffuse*DIFFUSE + ambient*AMBIENT);
}

void main()
{
  vec3 baseCol = vec3(0.5f,0.9f,0.4f) * 1.0/abs(sin(vTexCoord.t*50.0 + t/20.0));
  fColor = vec4(computeLighting(baseCol, vNormal, normalize(vec3(1.0,1.0,1.0))) +
		0.5*computeLighting(baseCol, vNormal, normalize(vec3(1.0,-1.0,-1.0))), 1.0);
}
