varying vec3 vNormal, vPosition;

void initLighting(vec3 position);
vec4 computeLighting(vec3 normal, vec3 position, vec4 diffuseScale, vec4 specularScale, vec4 ambientScale);

void main()
{
  initLighting(vPosition);
  //gl_FragColor = vec4(1.0,0.0,0.0,1.0);
  gl_FragColor = vec4(computeLighting(vNormal, vPosition, gl_FrontMaterial.diffuse, gl_FrontMaterial.specular, vec4(1.0)).rgb, 1.0);
}
