varying vec3 vNormal, vPosition;

void main()
{
  
  vNormal = normalize(gl_Normal);
  vPosition = vec3(gl_Vertex);
  gl_Position = ftransform();
}
