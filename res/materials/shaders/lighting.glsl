uniform vec4 lightPosition;
uniform vec4 cameraPosition;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;
uniform vec4 lightDirection;
uniform vec4 lightAmbient;

#define roughness_val 1.0/gl_FrontMaterial.shininess

vec3 lightDir;
vec3 halfVector;
vec3 viewVector;

vec3 getViewVector()
{
  return viewVector;
}

void initLighting(vec3 position)
{ 
  //toCamera = normalize(vPosition - vec3(cameraPosition));
  viewVector = normalize(vec3(cameraPosition) - position);
}

vec4 computeLighting(vec3 normal, vec3 position, vec4 diffuseScale, vec4 specularScale, vec4 ambientScale)
{
  vec4 result = vec4(0.0);
  
  //lightDir = vec3(lightPosition[i]) - position;
  //float lightDist = length(lightDir);
  lightDir = normalize(-lightDirection);
  halfVector = normalize(lightDir + viewVector);
  
  float diffuse = max(dot(normal, lightDir),0.0);
  
  //cook-torrance specular highlight computation
  //vec3  half_vec = normalize( _view + _light );
  
  float NdotL    = max( dot( normal, lightDir ), 0.0 );
  float NdotV    = max( dot( normal, viewVector ), 0.0 );
  float NdotH    = max( dot( normal, halfVector ), 1.0e-7 );
  float VdotH    = max( dot( viewVector,   halfVector ), 0.0 );
  
  float geometric = 2.0 * NdotH / VdotH;
  geometric = min( 1.0, geometric * min(NdotV, NdotL) );
  
  float r_sq          = roughness_val * roughness_val;
  float NdotH_sq      = NdotH * NdotH;
  float NdotH_sq_r    = 1.0 / (NdotH_sq * r_sq);
  float roughness_exp = (NdotH_sq - 1.0) * ( NdotH_sq_r );
  float roughness     = exp(roughness_exp) * NdotH_sq_r / (4.0 * NdotH_sq );
  
  float fresnel       = 1.0 / (1.0 + NdotV);
  
  float specular = min(1.0, (fresnel * geometric * roughness) / (NdotV * NdotL + 1.0e-7));
  
  return (diffuseScale*lightDiffuse*diffuse + specularScale*lightSpecular*specular) + ambientScale*lightAmbient;
}
