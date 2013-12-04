#version 410
in vec4 vertex_location;
out vec4 color;
uniform float t;
uniform mat4 modelViewMat;

void main()
{
	color = vec4(vertex_location.xy,0.0,0.0f);
}
