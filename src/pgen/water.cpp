#include "pgen/water.hpp"

#include <noise/noise.h>

void pgen::genWater(const int xres, const int yres, const int zres, float* hmap)
{
  const int octaves = log2(max(max(xres,yres),zres))/4;
  noise::module::Perlin noise_main;
  noise_main.SetFrequency(4.0);
  noise_main.SetOctaveCount(octaves);
  noise_main.SetNoiseQuality(noise::QUALITY_BEST);
  noise_main.SetSeed(time(NULL));
  
  for(int x=0; x<xres; x++)
    {
      for(int y=0; y<yres; y++)
	{
	  for(int z=0; z<zres; z++)
	    {
	      hmap[x*yres*zres + y*zres + z] = (noise_main.GetValue(x/(float)xres,y/(float)yres,z/(float)zres) + noise_main.GetValue(xres-x,yres-y,zres-z))*0.5f;
	    }
	}
    }
}

void pgen::cacheWater(const int res, const int tres, float* hmap, glm::vec3* nmap, glm::vec3* tmap, glm::vec3* bmap)
{
  //bake the save
  FILE* f = fopen("res/water/water_hmap.float","rb");
  if(f == NULL)
    {
      std::cout << "Generating water..." << std::endl;
      f = fopen("res/water/water_hmap.float","wb");
      genWater(res,res,tres,hmap);
      std::cout << "Computing gradient..." << std::endl;
      computeGradient3D(hmap,res,res,tres,tmap,bmap,nmap);
      std::cout << "Saving..." << std::endl;
      fwrite(hmap,sizeof(float),sizeof(float)*res*res*tres, f);
      fclose(f);
      f = fopen("res/water/water_tmap.float","wb");
      fwrite(tmap,sizeof(glm::vec3),sizeof(glm::vec3)*res*res*tres, f);
      fclose(f);
      f = fopen("res/water/water_bmap.float","wb");
      fwrite(bmap,sizeof(glm::vec3),sizeof(glm::vec3)*res*res*tres, f);
      fclose(f);
      f = fopen("res/water/water_nmap.float","wb");
      fwrite(nmap,sizeof(glm::vec3),sizeof(glm::vec3)*res*res*tres, f);
      fclose(f);
      std::cout << "Done" << std::endl;
    }
  else
    {
      fclose(f);
      f = fopen("res/water/water_hmap.float","rb");
      fread(hmap,sizeof(float),sizeof(float)*res*res*tres, f);
      fclose(f);
      f = fopen("res/water/water_tmap.float","rb");
      fread(tmap,sizeof(glm::vec3),sizeof(glm::vec3)*res*res*tres, f);
      fclose(f);
      f = fopen("res/water/water_bmap.float","rb");
      fread(bmap,sizeof(glm::vec3),sizeof(glm::vec3)*res*res*tres, f);
      fclose(f);
      f = fopen("res/water/water_nmap.float","rb");
      fread(nmap,sizeof(glm::vec3),sizeof(glm::vec3)*res*res*tres, f);
      fclose(f);
    }
}
