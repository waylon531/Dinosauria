#version 410

uniform int waterFlag;

in vec3 vPosition;
in vec3 vNormal;

out vec4 fColor;
out vec4 fNormal;
out vec4 fPosition;
out vec4 fParams;

void main()
{
  if(waterFlag==1)
    {
      if(vPosition.y < 0.0f)
	{
	  discard;
	}
    }
  if(waterFlag==2)
    {
      if(vPosition.y > 0.0f)
	{
	  discard;
	}
    }
  fColor = vec4(.75,.2,.5, 1.f);
  fNormal = vec4(vNormal,1.f);
  fPosition = vec4(vPosition, 1.);
  fParams = vec4(.7, .2, 10., 1.);
}
