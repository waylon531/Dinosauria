#version 410

uniform sampler2D tex_color;
uniform sampler2D tex_normal;
uniform sampler2D tex_params;
uniform sampler2D tex_depth;
uniform sampler2D tex_position;

in vec2 vTexCoord;

out vec4 fColor;
out vec4 fNormal;
out vec4 fPosition;
out vec4 fParams;

#define PIXEL_X (1.f/1280.f)
#define PIXEL_Y (1.f/1024.f)

#define THRESHOLD 1.0
#define BLUR_SPAN 10
#define STOP_THRESHOLD 0.25
#define FALLOFF 0.8
//#define DISPLAY_EDGES

float lum(vec4 v)
{
  return (v.x+v.y+v.z)/3.f;
}

vec4 FXAA(sampler2D tex)
{
  //do local contrast check
  vec4 center = texture2D(tex, vTexCoord.st);
  vec4 north = texture2D(tex, vec2(vTexCoord.s,vTexCoord.t+PIXEL_Y));
  vec4 south = texture2D(tex, vec2(vTexCoord.s,vTexCoord.t-PIXEL_Y));
  vec4 east = texture2D(tex, vec2(vTexCoord.s+PIXEL_X,vTexCoord.t));
  vec4 west = texture2D(tex, vec2(vTexCoord.s-PIXEL_X,vTexCoord.t));
  float fcenter = lum(center);
  float fnorth = lum(north);
  float fsouth = lum(south);
  float feast = lum(east);
  float fwest = lum(west);
  if(max(max(max(fcenter,fnorth),max(fsouth,feast)),fwest) - min(min(min(fcenter,fnorth),min(fsouth,feast)),fwest) > THRESHOLD)
    {
      //get direction
      bool horizontal;
      if(max(max(fcenter,fnorth),fsouth) - min(min(fcenter,fnorth),fsouth) > max(max(fcenter,feast),fwest) - min(min(fcenter,feast),fwest))
	{
	  horizontal = false;
	}
      else
	{
	  horizontal = true;
	}
      if(horizontal)
	{
#ifdef DISPLAY_EDGES
	  return vec4(1,0,0,1);
#endif
	  vec4 final = center;
	  float samples=1;
	  float weight = 1.f;
	  for(int i=1; i<=BLUR_SPAN; i++)
	    {
	      vec4 tmp = texture2D(tex, vec2(vTexCoord.s+PIXEL_X*i, vTexCoord.t));
	      final += tmp*weight;
	      samples+=weight;
	      weight *= FALLOFF;
	      if(abs(lum(tmp/float(samples)) - fcenter) > STOP_THRESHOLD)
		{
		  break;
		}
	    }
	  weight = 1.f;
	  for(int i=-1; i>=-BLUR_SPAN; i--)
	    {
	      vec4 tmp = texture2D(tex, vec2(vTexCoord.s+PIXEL_X*i, vTexCoord.t));
	      final += tmp*weight;
	      samples+=weight;
	      weight *= FALLOFF;
	      if(abs(lum(tmp/float(samples)) - fcenter) > STOP_THRESHOLD)
		{
		  break;
		}
	    }
	  final /= float(samples);
	  return final;
	}
      else
	{
#ifdef DISPLAY_EDGES
	  return vec4(0,1,0,1);
#endif
	  vec4 final = center;
	  float samples=1;
	  float weight = 1.f;
	  for(int i=1; i<=BLUR_SPAN; i++)
	    {
	      vec4 tmp = texture2D(tex, vec2(vTexCoord.s, vTexCoord.t+PIXEL_Y*i));
	      final += tmp*weight;
	      samples+=weight;
	      weight *= FALLOFF;
	      if(abs(lum(tmp/float(samples)) - fcenter) > STOP_THRESHOLD)
		{
		  break;
		}
	    }
	  weight = 1.f;
	  for(int i=-1; i>=-BLUR_SPAN; i--)
	    {
	      vec4 tmp = texture2D(tex, vec2(vTexCoord.s, vTexCoord.t+PIXEL_Y*i));
	      final += tmp*weight;
	      samples+=weight;
	      weight *= FALLOFF;
	      if(abs(lum(tmp/float(samples)) - fcenter) > STOP_THRESHOLD)
		{
		  break;
		}
	    }
	  final /= float(samples);
	  return final;
	}
    }
  else
    {
#ifdef DISPLAY_EDGES
      return vec4(0,0,1,1);
#endif
      return center;
    }
}

void main()
{
  //COLOR
  
    
  fColor = FXAA(tex_color);
  fNormal = FXAA(tex_normal);
  fPosition = FXAA(tex_position);
  fParams = FXAA(tex_params);
}
