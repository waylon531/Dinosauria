#include "pgen/terrain.hpp"
#include <noise/noise.h>

void pgen::generateTerrain(const int res, float* hmap, float* bmap, float* value, Callback& callback)
{
  const int octaves = log2(res);
  noise::module::RidgedMulti noise_mountains;
  noise_mountains.SetFrequency(2.0);
  noise_mountains.SetOctaveCount(octaves);
  noise_mountains.SetNoiseQuality(noise::QUALITY_BEST);
  noise_mountains.SetSeed(time(NULL));

  noise::module::Perlin noise_hills_base;
  noise_hills_base.SetFrequency(2.0);
  noise_hills_base.SetOctaveCount(octaves);
  noise_hills_base.SetNoiseQuality(noise::QUALITY_BEST);
  noise_hills_base.SetSeed(time(NULL));
  noise::module::ScaleBias noise_hills;
  noise_hills.SetSourceModule(0,noise_hills_base);
  noise_hills.SetScale(0.3);
  noise_hills.SetBias(-0.75);

  noise::module::Perlin noise_mix;
  noise_mix.SetFrequency(1.0);
  noise_mix.SetOctaveCount(octaves);
  noise_mix.SetNoiseQuality(noise::QUALITY_BEST);
  noise_mix.SetSeed(time(NULL));
  noise_mix.SetPersistence(0.25);
  
  noise::module::Select noise_final;
  noise_final.SetSourceModule(0,noise_hills);
  noise_final.SetSourceModule(1,noise_mountains);
  noise_final.SetControlModule(noise_mix);
  noise_final.SetBounds(0.0,1000.0);
  noise_final.SetEdgeFalloff(0.125);
  
  //float* a = new float[res*res];
  for(int x=0; x<res; x++)
    {
      for(int y=0; y<res; y++)
	{
	  hmap[x*res+y] = noise_final.GetValue(x/(float)res+0.5,y/(float)res+0.5,0.5);
	  bmap[x*res+y] = (noise_mix.GetValue(x/(float)res+0.5,y/(float)res+0.5,0.5)+1.0)/2.0;											    
	}
      if(x%10==0)
	{
	  *value = x/(float)res;
	  callback.call();
	}
    }
 
}
