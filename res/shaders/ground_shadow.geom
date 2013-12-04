#version 410
#extension GL_EXT_geometry_shader4 : enable 

layout (triangles)
in;

layout (triangle_strip, max_vertices=3)
out;

in vec2 eTexCoord[];
in vec3 ePosition[];
in vec4 ePositionSS[];

void drawLine(int i)
{
  gl_Position = gl_in[i].gl_Position;
  EmitVertex();
}

void main()
{
  int i;
  for(i=0; i<gl_VerticesIn; i++)
    {
      drawLine(i);
    }
}
