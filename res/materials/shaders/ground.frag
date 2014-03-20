uniform sampler2D diffuseMap, bumpMap, normalMap;

varying vec3 vPosition;
varying vec2 vTexCoord;

void initLighting(vec3 position);
vec4 computeLighting(vec3 normal, vec3 position, vec4 diffuseScale, vec4 specularScale, vec4 ambientScale);

vec3 getViewVector();

#ifdef USE_PARALLAXMAP

#define HEIGHT 0.0225
#define STEP_SIZE .001
#define STEPS 32

vec2 parallaxMapping(vec2 coord)
{
  float height = 0.0;//HEIGHT * texture2D(bumpMap, coord).x;
  vec2 newCoord = coord;// + viewVector.xz * height;
  float y = height;
  for(int i=0; i<STEPS; i++)
    {
      newCoord += getViewVector().xz * STEP_SIZE;
      y -= getViewVector().y * STEP_SIZE;
      height = HEIGHT * (1.0 - texture2D(bumpMap, newCoord).x) - HEIGHT;
      if(height > y)
	{
	  return newCoord;
	}
    }
  return newCoord;
}

#endif

void main()
{
  initLighting(vPosition);
  vec2 texCoord = 0.01*vPosition.xz;
  #ifdef USE_PARALLAXMAP
  texCoord = parallaxMapping(texCoord);
  #endif
  //gl_FragColor = vec4(mod(texCoord.x,1.0), mod(texCoord.y,1.0), 0.0, 1.0);
  //return;
  #ifdef USE_NORMALMAP
  vec3 normal = texture2D(normalMap, texCoord).xzy;
  #else
  vec3 normal = vec3(0.0,1.0,0.0);
  #endif
  vec4 diffuse = texture2D(diffuseMap, texCoord);
  vec4 lighting = computeLighting(normal, vPosition, diffuse, diffuse, diffuse);
  gl_FragColor = vec4(lighting.xyz, 1.0);
}
