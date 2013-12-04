#version 410

uniform sampler2D tex_globe;

out vec4 fragColor;
in vec3 gFacetNormal;
in vec3 gTriDistance;
in vec3 gPatchDistance;
in float gPrimitive;
//in vec2 gTexCoord;

uniform mat4 modelView;
uniform float t;

float amplify(float d, float scale, float offset)
{
  d = scale * d + offset;
  d = clamp(d,0,1);
  d = 1 - exp2(-2*d*d);
  return d;
}

#define SPECULAR 0.5
#define DIFFUSE 0.5
#define AMBIENT 0.2
vec3 computeLighting(vec3 base)
{
  vec3 lightDir = normalize(vec3(1,1,1));
  vec3 eyeDir = normalize(vec3(0,0,1));
  float specular = clamp(dot(eyeDir,reflect(-lightDir,gFacetNormal)),0,1);
  specular = pow(specular,10);
  float diffuse = clamp(dot(gFacetNormal,lightDir),0,1);
  float ambient = 1.0;
  return base*(specular*SPECULAR + diffuse*DIFFUSE + ambient*AMBIENT);
}

void main()
{
  //vec3 color = texture2D(tex_globe, gPatchDistance.xy).xyz;
  //vec3 color = vec3(gTexCoord.xy,1);
  vec3 color = vec3(0.5,0.9,1);
  color = computeLighting(color);
  float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
  float d2 = min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z);
  color = mix(color,amplify(d1,100,-1.0) * amplify(d2, 200, -1.0) * color, 0.25);
  fragColor = vec4(color,1);
}
