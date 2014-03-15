#include "graphics/context.hpp"

//std includes
#include <cstdlib>
#include <unistd.h> //REMOVE (only used for usleep so keep for now)
//external includes
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreWindowEventUtilities.h>
#include <OISInputManager.h>
#include <CEGUIOgreRenderer.h>
#include <CEGUIFontManager.h>
#include <CEGUISchemeManager.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <CEGUIImageset.h>
//#include <CEGUIWidgetModule.h>
//custom includes
#include "util.hpp"
#include "engine/state_manager.hpp"

void graphics::OgreEngine::setupGUIMgr()
{
  window->removeAllViewports();
  window->addViewport(cam_gui);
}

void graphics::OgreEngine::startup()
{
  isInitialized = true;
  root = new Ogre::Root("ogre_cfg/plugins.cfg","ogre_cfg/ogre.cfg",LOG_FILENAME);

  if (root->showConfigDialog())
    {
      window = root->initialise(true, "OTST");
    }
  else
    {
      exit(-1);
    }

  //load resources (borrowed from OGRE wiki tutorial framework)
  {
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load("ogre_cfg/resources.cfg");   
    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
      {
	secName = seci.peekNextKey();
	Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
	Ogre::ConfigFile::SettingsMultiMap::iterator i;
	for (i = settings->begin(); i != settings->end(); ++i)
	  {
	    typeName = i->first;
	    archName = i->second;
	    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
	  }
      }
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  }
  
  //create scene managers
  sceneMgr_gui = root->createSceneManager(Ogre::ST_GENERIC);
  cam_gui = sceneMgr_gui->createCamera("MAIN");
  cam_gui->setPosition(0,0,300);
  cam_gui->lookAt(0,0,0);
  setupGUIMgr();
  //setup CEGUI
  GUIrenderer = &CEGUI::OgreRenderer::bootstrapSystem();
#ifdef DEBUG
  CEGUI::Logger::getSingleton().setLogFilename("cegui.log");
#endif
  CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Insane);
  CEGUI::Imageset::setDefaultResourceGroup("GUI");
  CEGUI::Font::setDefaultResourceGroup("GUI");
  CEGUI::Scheme::setDefaultResourceGroup("GUI");
  //CEGUI::WidgetLookManager::setDefaultResourceGroup("GUI");
  CEGUI::WindowManager::setDefaultResourceGroup("GUI");
  
  CEGUI::SchemeManager::getSingleton().create((CEGUI::utf8*)"TaharezLook.scheme");
  CEGUI::FontManager::getSingleton().create("DejaVuSans-10.font");
  CEGUI::System::getSingleton().setDefaultFont((CEGUI::utf8*)"DejaVuSans-10");
  CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook","MouseArrow");
  //TEMPORARY
  //CEGUI::Window* guiRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout("sheets/main_menu.layout");
  //CEGUI::System::getSingleton().setGUISheet(guiRoot);
  
  //ENDTEMPORARY
  CEGUI::System::getSingleton().injectMousePosition((float)window->getWidth()/2.f, (float)window->getHeight()/2.f);
      
  //register for input from OIS
  OIS::ParamList pl;
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;
  window->getCustomAttribute("WINDOW", &windowHnd);
  windowHndStr << windowHnd;
  pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
  pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
  pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
  inputMgr = OIS::InputManager::createInputSystem( pl );
  keyboard = static_cast<OIS::Keyboard*>(inputMgr->createInputObject( OIS::OISKeyboard, true ));
  mouse = static_cast<OIS::Mouse*>(inputMgr->createInputObject( OIS::OISMouse, true ));
  mouse->setEventCallback(this);
  keyboard->setEventCallback(this);
  //Ogre::WindowEventUtilities::addWindowEventListener(window, this);
  
  //create frame listener
  root->addFrameListener(this);
}

void graphics::OgreEngine::shutdown()
{ 
  delete root;
}

void graphics::OgreEngine::startRenderLoop()
{
  root->startRendering();
}

bool graphics::OgreEngine::frameRenderingQueued(const Ogre::FrameEvent& e)
{
  keyboard->capture();
  mouse->capture();
  CEGUI::System::getSingleton().injectTimePulse(e.timeSinceLastFrame);
  engine::StateManager::getSingleton().onTick(e.timeSinceLastFrame);
  if(window->isClosed() || engine::StateManager::getSingleton().isRunning==false) return false;
  //cap framerate
  if(e.timeSinceLastFrame < 15.0)
    {
      usleep(15000 - int(1000 * e.timeSinceLastFrame));
    }
  
  return true;
}

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
  switch (buttonID)
    {
    case OIS::MB_Left:
      return CEGUI::LeftButton;
    case OIS::MB_Right:
      return CEGUI::RightButton;
    case OIS::MB_Middle:
      return CEGUI::MiddleButton;
    default:
      return CEGUI::LeftButton;
    }
}

bool graphics::OgreEngine::mouseMoved(const OIS::MouseEvent& e)
{
  //const OIS::MouseState state = mouse->getMouseState();
  CEGUI::System::getSingleton().injectMousePosition(e.state.X.abs, e.state.Y.abs);
  engine::StateManager::getSingleton().onMouseMove(e);
  return true;
}

bool graphics::OgreEngine::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
  engine::StateManager::getSingleton().onMouseDown(e,id);
  return true;
}

bool graphics::OgreEngine::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
  engine::StateManager::getSingleton().onMouseUp(e,id);
  return true;
}

bool graphics::OgreEngine::keyPressed(const OIS::KeyEvent& e)
{
  if(e.text == 'q') exit(-1);
  CEGUI::System::getSingleton().injectKeyDown(e.key);
  CEGUI::System::getSingleton().injectChar(e.text);
  engine::StateManager::getSingleton().onKeydown(e);
  return true;
}

bool graphics::OgreEngine::keyReleased(const OIS::KeyEvent& e)
{
  CEGUI::System::getSingleton().injectKeyUp(e.key);
  engine::StateManager::getSingleton().onKeyup(e);
  return true;
}
