local CEGUISystem = CEGUI.System:getSingleton()
local CEGUIWindowManager = CEGUI.WindowManager:getSingleton()

class 'MainMenuState' (engine.GameState)

function StartGame_onClick(args)
   StateManager:switchState("test")
   return true
end

function Options_onClick(args)
   StateManager:pushState("options")
   return true
end

function Quit_onClick(args)
   StateManager.isRunning = false
   return true
end

function MainMenuState:getName()
   return "main_menu"
end

function MainMenuState:__init()
   engine.GameState.__init(self)
   self.guiRoot = CEGUIWindowManager:loadWindowLayout("main_menu.layout")

   CEGUIWindowManager:getWindow("MainMenu/StartGame"):subscribeEvent("Clicked","StartGame_onClick")
   CEGUIWindowManager:getWindow("MainMenu/Options"):subscribeEvent("Clicked","Options_onClick")
   CEGUIWindowManager:getWindow("MainMenu/Quit"):subscribeEvent("Clicked","Quit_onClick")
end

function MainMenuState:enter()
   print("Entering MainMenuState")
   --setGUILayout(self.guiRoot)
   CEGUISystem:setGUISheet(self.guiRoot)
   OgreEngine:setupGUIMgr()
end

function MainMenuState:exit()
end

function MainMenuState:pause()
   self:exit()
end

function MainMenuState:resume()
   self:enter()
end

