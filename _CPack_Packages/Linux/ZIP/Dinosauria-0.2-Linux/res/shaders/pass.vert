#version 410
in vec3 position;
out vec4 vert;
uniform float t;
uniform mat4 modelViewMat;

//\include res/shaders/test.slh

void main()
{
  vert = modelViewMat * vec4(position,1.0f);
  gl_Position.xy = vert.xy;
  gl_Position.z = 0.0f;
  gl_Position.w = 1.0f;
}
