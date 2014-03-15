varying vec3 vPosition;
varying vec2 vTexCoord;

void main()
{
  vPosition = vec3(gl_Vertex);
  vTexCoord = gl_TexCoord[0].xy;
  gl_Position = ftransform();
}
