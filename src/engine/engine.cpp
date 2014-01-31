#include "engine/engine.hpp"

engine::EngineManager::EngineManager()
{
}

engine::EngineManager::~EngineManager()
{
  for(int i=0; i<systems.size(); i++)
    {
      //delete systems[i];
    }
}

void engine::EngineManager::update()
{
  for(int i=0; i<systems.size(); i++)
    {
      std::vector<Entity> etmp;
      for(int ii=0; ii<entities.size(); ii++)
	{
	  if(entities[ii].checkSig(systems[i]->filter))
	    {
	      etmp.push_back(entities[ii]);
	    }
	}
      systems[i]->runSystem(etmp);
    }
}
