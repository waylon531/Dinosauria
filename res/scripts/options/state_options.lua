local CEGUISystem = CEGUI.System:getSingleton()
local CEGUIWindowManager = CEGUI.WindowManager:getSingleton()

class 'OptionsState' (engine.GameState)

function Close_onClick(args)
   StateManager:popState()
   return true
end

function Graphics_onStateChange(args)
   local id = CEGUI.toRadioButton(CEGUI.toWindowEventArgs(args).window):getSelectedButtonInGroup():getID()
   if id==0 then
      engine.setUserSetting("graphicsMode","low")
   end
   if id==1 then
      engine.setUserSetting("graphicsMode","medium")
   end
   if id==2 then
      engine.setUserSetting("graphicsMode","high")
   end
   if id==3 then
      engine.setUserSetting("graphicsMode","extreme")
   end

   StateManager:onGraphicsSettings()
   engine.saveSettings()

   return true
end
   
function OptionsState:getName()
   return "options"
end

local function setRadioButton(name)
   CEGUI.toRadioButton(CEGUIWindowManager:getWindow(name)):setSelected(true)
end   

function OptionsState:__init()
   engine.GameState.__init(self)
   self.guiRoot = CEGUIWindowManager:loadWindowLayout("options.layout")

   CEGUIWindowManager:getWindow("Options/Close"):subscribeEvent("Clicked", "Close_onClick")
   CEGUIWindowManager:getWindow("Options/GraphicsLow"):subscribeEvent("SelectStateChanged", "Graphics_onStateChange")
   CEGUIWindowManager:getWindow("Options/GraphicsMedium"):subscribeEvent("SelectStateChanged", "Graphics_onStateChange")
   CEGUIWindowManager:getWindow("Options/GraphicsHigh"):subscribeEvent("SelectStateChanged", "Graphics_onStateChange")
   CEGUIWindowManager:getWindow("Options/GraphicsExtreme"):subscribeEvent("SelectStateChanged", "Graphics_onStateChange")

   if engine.getUserSetting("graphicsMode")=="low" then
      setRadioButton("Options/GraphicsLow")
   end
   if engine.getUserSetting("graphicsMode")=="medium" then
      setRadioButton("Options/GraphicsMedium")
   end
   if engine.getUserSetting("graphicsMode")=="high" then
      setRadioButton("Options/GraphicsHigh")
   end
   if engine.getUserSetting("graphicsMode")=="extreme" then
      setRadioButton("Options/GraphicsExtreme")
   end
end

function OptionsState:enter()
   CEGUISystem:setGUISheet(self.guiRoot)
   OgreEngine:setupGUIMgr()
end

function OptionsState:exit()
end

function OptionsState:pause()
   self:exit()
end

function OptionsState:resume()
   self:enter()
end
