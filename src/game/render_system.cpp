#include "game/render_system.hpp"

RenderSystem::RenderSystem(std::shared_ptr<graphics::World> w) : world(w)
{
  filter = engine::getEntitySig("renderable");
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::runSystem(std::vector<engine::Entity>& entities)
{
  const unsigned int c_rendObj = engine::getComponentID("rendObject");
  std::vector<std::shared_ptr<graphics::RenderableObjectExt>> objects;
  for(int i=0; i<entities.size(); i++)
    {
      std::shared_ptr<graphics::RenderableObjectExt> object = entities[i].getComponent("rendObject").getData<std::shared_ptr<graphics::RenderableObjectExt>>();
      assert(object);
      if(entities[i].hasComponent("m_model"))
	{
	  *(entities[i].getComponent("m_model").getData<std::shared_ptr<glm::mat4>>()) = glm::translate(entities[i].getComponent("pos").getData<glm::vec3>()) * glm::rotate(entities[i].getComponent("rot").getData<glm::vec3>().z, glm::vec3(0.f,0.f,1.f));
	}
      objects.push_back(object);
    }
  world->setMeshList(objects);
}
      
