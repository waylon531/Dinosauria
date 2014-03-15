//file: graphics/context.hpp
//purpose: OGRE context manegment

#pragma once

//external includes
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreFrameListener.h>

namespace Ogre
{
  class RenderWindow;
  class Root;
  class SceneManager;
  class Camera;
};

namespace CEGUI
{
  class OgreRenderer;
};

namespace OIS
{
  class InputManager;
};

namespace graphics
{
  /** The OGRE engine */
  class OgreEngine : public OIS::MouseListener, public OIS::KeyListener, public Ogre::FrameListener
  {
  private:

    /** If this engine has been initialized yet */
    bool isInitialized;
    
  public:

    /** The OGRE rendering window to use */
    Ogre::RenderWindow* window;

    /** The OGRE root to use */
    Ogre::Root* root;

    /** The scene manager used for GUI */
    Ogre::SceneManager* sceneMgr_gui;

    /** The CEGUI renderer */
    CEGUI::OgreRenderer* GUIrenderer;

    /** The OIS input manager object */
    OIS::InputManager* inputMgr;

    /** The OIS mouse object */
    OIS::Mouse* mouse;

    /** The OIS keyboard object */
    OIS::Keyboard* keyboard;

    /** The OGRE GUI camera object */
    Ogre::Camera* cam_gui;
    
    /** Get a singleton instance of this class
     * @returns only instance of OgreEngine */
    static inline OgreEngine& getSingleton()
    {
      static OgreEngine engine;
      return engine;
    }

    /** @internal Initialize the engine */
    OgreEngine()
    {
      isInitialized = false;
    }

    /** Setup the GUI scene manager camera */
    void setupGUIMgr();

    /** Destroy the engine and preform shutdown */
    void shutdown();

    /** Run the engine startup phase */
    void startup();

    /** Begin the OGRE render loop */
    void startRenderLoop();

    /** @internal */
    bool frameRenderingQueued(const Ogre::FrameEvent& e);

    /** @internal */
    bool mouseMoved(const OIS::MouseEvent& e);

    /** @internal */
    bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID);

    /** @internal */
    bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID);

    /** @internal */
    bool keyPressed(const OIS::KeyEvent& e);

    /** @internal */
    bool keyReleased(const OIS::KeyEvent& e);
    
  };
};
