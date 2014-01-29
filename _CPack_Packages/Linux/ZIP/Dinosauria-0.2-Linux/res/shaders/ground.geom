#version 410
#extension GL_EXT_geometry_shader4 : enable 

layout (triangles)
in;

layout (triangle_strip, max_vertices=3)
out;

/*in gl_PerVertex
{
  vec4 gl_Position;
  } gl_in[];*/

in vec2 eTexCoord[];
in vec3 eNormal[];
in vec3 ePosition[];
in vec4 ePositionSS[];

/*out gl_PerVertex
{
  vec4 gl_Position;
  };*/

out vec2 gTexCoord;
out vec3 gNormal;
out vec3 gPosition;
out vec3 gPatchDist;
out vec4 gPositionSS;

void drawLine(int i)
{
  gl_Position = gl_in[i].gl_Position;
  gTexCoord = eTexCoord[i];
  gNormal = eNormal[i];
  gPosition = ePosition[i];
  gPositionSS = ePositionSS[i];
  if(i%3==0) gPatchDist = vec3(1,0,0);
  else if(i%3==1) gPatchDist = vec3(0,1,0);
  else gPatchDist = vec3(0,0,1);
  EmitVertex();
}

void main()
{
  int i;
  for(i=0; i<gl_VerticesIn; i++)
    {
      drawLine(i);
      //if(i/4.0 == i/4) EndPrimitive();
    }
}
