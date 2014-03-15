#pragma once

//custom includes
#include "engine/state.hpp"

namespace Ogre
{
  class SceneManager;
  class Camera;
};

namespace CEGUI
{
  class Window;
};

/** A prototype state */
class PrototypeState: public engine::GameState
{
private:

  /** The OGRE scene manager */
  Ogre::SceneManager* sceneMgr;

  /** The OGRE camera */
  Ogre::Camera* cam;

  /** The gui layout */
  CEGUI::Window* guiRoot;

public:

  std::string getName() const
  {
    return "test";
  }
  
  /** Initialize */
  PrototypeState();

  /** Destroy */
  ~PrototypeState();

  void enter();
  void exit();
  void pause();
  void resume();

  void onTick(const float pulse);
  void onGraphicsSettings();
};
