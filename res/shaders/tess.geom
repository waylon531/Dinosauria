#version 410
//simple tesselation shader

layout(triangles) in;
layout(line_strip, max_vertices=24) out;

in vec4 vert[];
out vec4 vertex_location;

void drawVert(vec4 v)
{
	gl_Position = v;
	vertex_location = v;
	EmitVertex();
}

void drawTri(vec4 v0, vec4 v1, vec4 v2)
{
	drawVert(v0);
	drawVert(v1);
	EndPrimitive();
	drawVert(v1);
	drawVert(v2);
	EndPrimitive();
	drawVert(v2);
	drawVert(v0);
	EndPrimitive();
}

void main()
{
	int i;
	vec4 p0,p1,p2;
	p0 = mix(vert[0],vert[1],.5);
	p1 = mix(vert[1],vert[2],.5);
	p2 = mix(vert[2],vert[0],.5);
	vec4 v0=vert[0];
	vec4 v1=vert[1];
	vec4 v2=vert[2];
	drawTri(v0,p0,p2);
	drawTri(v1,p1,p0);
	drawTri(v2,p2,p1);
	drawTri(p0,p1,p2);
}
