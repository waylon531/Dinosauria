#include "game/state_test.hpp"

//external includes
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreLight.h>
#include <OgreCamera.h>
#include <OgreRoot.h>
#include <Ogre.h>
#include <OgreRenderWindow.h>
#include <OgreMaterialManager.h>
#include <OgreMeshManager.h>
#include <OgrePlane.h>
#include <OgreAnimationState.h>
#include <OgreAnimation.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
//custom includes
#include "engine/state_manager.hpp"
#include "engine/settings.hpp"
#include "graphics/context.hpp"
#include "util.hpp"

PrototypeState::PrototypeState()
{
  guiRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout("test.layout");
  
  sceneMgr = graphics::OgreEngine::getSingleton().root->createSceneManager(Ogre::ST_GENERIC);

  sceneMgr->setAmbientLight(Ogre::ColourValue(.1f,.1f,.1f));
  sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
  //sceneMgr->setShadowTextureSettings(512, 16, Ogre::PF_FLOAT16_R, 50);
  sceneMgr->setShadowTextureCount(2);
  
  Ogre::Entity* mesh = sceneMgr->createEntity("Mesh0", "dei_body.mesh");
  mesh->setCastShadows(true);
  Ogre::SceneNode* meshNode = sceneMgr->getRootSceneNode()->createChildSceneNode("Mesh0Node");
  meshNode->attachObject(mesh);
  Ogre::AnimationState* animState = mesh->getAnimationState("waveTail");
  animState->setEnabled(true);
  animState->setLoop(true);

  Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0.0f);
  Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 500, 500, 1, 1, true, 1, 5.f, 5.f, Ogre::Vector3::UNIT_Z);
  Ogre::Entity* ground = sceneMgr->createEntity("Ground", "ground");
  ground->setCastShadows(false);
  ground->setMaterialName("ground");
  Ogre::SceneNode* groundNode = sceneMgr->getRootSceneNode()->createChildSceneNode("GroundNode");
  groundNode->attachObject(ground);
  groundNode->translate(0,-20,0);

  Ogre::Light* l1 = sceneMgr->createLight("Light1");
  l1->setType(Ogre::Light::LT_DIRECTIONAL);
  l1->setDirection(0,-1,0);
  l1->setDiffuseColour(1,1,1);
  l1->setSpecularColour(1,1,1);

  /*Ogre::Light* l0 = sceneMgr->createLight("Light0");
  l0->setType(Ogre::Light::LT_DIRECTIONAL);
  l0->setDirection(0,-1,1);
  l0->setDiffuseColour(1,2,1);
  l0->setSpecularColour(1,.5,.5);*/
  
  cam = sceneMgr->createCamera("PlayerCam");
  cam->setPosition(90,90,90);
  cam->lookAt(0,0,0);
  cam->setNearClipDistance(3);
  cam->setFarClipDistance(500);
}

PrototypeState::~PrototypeState()
{
  delete sceneMgr;
}

void PrototypeState::enter()
{
  CEGUI::System::getSingleton().setGUISheet(guiRoot);
  graphics::OgreEngine::getSingleton().window->removeAllViewports();
  graphics::OgreEngine::getSingleton().window->addViewport(cam);
  Ogre::Animation::setDefaultInterpolationMode(Ogre::Animation::IM_SPLINE);
  Ogre::Animation::setDefaultRotationInterpolationMode(Ogre::Animation::RIM_SPHERICAL);
  cam->getViewport()->setMaterialScheme(engine::userSettings["graphicsMode"]);
}

void PrototypeState::exit()
{
  graphics::OgreEngine::getSingleton().window->removeAllViewports();
}

void PrototypeState::pause()
{
  exit();
}

void PrototypeState::resume()
{
  enter();
}

void PrototypeState::onTick(const float pulse)
{
  sceneMgr->getRootSceneNode()->getChild("Mesh0Node")->yaw(Ogre::Degree(-50.0*pulse));
  ((Ogre::Entity*)((Ogre::SceneNode*)sceneMgr->getRootSceneNode()->getChild("Mesh0Node"))->getAttachedObject("Mesh0"))->getAnimationState("waveTail")->addTime(pulse);
  std::cout << "\rfps: " << (1/(pulse)) << std::flush;
}

void PrototypeState::onGraphicsSettings()
{
  if(engine::userSettings["graphicsMode"] == "low")
    {
      sceneMgr->setShadowTextureSize(256*2);
      //sceneMgr->setShadowTextureFSAA(1);
    }
  else if(engine::userSettings["graphicsMode"] == "medium")
    {
      sceneMgr->setShadowTextureSize(512*2);
      //sceneMgr->setShadowTextureFSAA(1);
    }
  else if(engine::userSettings["graphicsMode"] == "high")
    {
      sceneMgr->setShadowTextureSize(1024*2);
      //sceneMgr->setShadowTextureFSAA(1);
    }
  else if(engine::userSettings["graphicsMode"] == "extreme")
    {
      sceneMgr->setShadowTextureSize(2048*2);
      //sceneMgr->setShadowTextureFSAA(2);
    }
}
