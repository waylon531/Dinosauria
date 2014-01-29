#version 410

uniform mat4 modelView;
uniform float t;
layout(triangles) 
in;
layout(triangle_strip, max_vertices=3) out;
in vec3 tePosition[3];
in vec3 tePatchDistance[3];
out vec3 gFacetNormal;
out vec3 gPatchDistance;
out vec3 gTriDistance;
//out vec2 gTexCoord;

////\include res/shaders/util.glh

/*vec2 toPolar(vec3 v)
{
  float theta = acos(v.z);
  float phi = atan(v.y/v.x);
  //return vec2(theta/M_PI, phi/(2*M_PI));
  return vec2(theta,phi);
  //return vec2(v.x,v.y);
  }*/

void main()
{
  vec3 A = tePosition[2] - tePosition[0];
  vec3 B = tePosition[0] - tePosition[1];
  gFacetNormal = normalize((inverse(transpose(modelView)) * vec4(normalize(cross(A,B)),0.0)).xyz);

  gPatchDistance = tePatchDistance[0];
  gTriDistance = vec3(1,0,0);
  //gTexCoord = toPolar(gl_in[0].gl_Position.xyz);
  gl_Position = gl_in[0].gl_Position; EmitVertex();

  gPatchDistance = tePatchDistance[1];
  gTriDistance = vec3(0,1,0);
  //gTexCoord = toPolar(gl_in[1].gl_Position.xyz);
  gl_Position = gl_in[1].gl_Position; EmitVertex();

  gPatchDistance = tePatchDistance[2];
  gTriDistance = vec3(0,0,1);
  //gTexCoord = toPolar(gl_in[2].gl_Position.xyz);
  gl_Position = gl_in[2].gl_Position; EmitVertex();
}

