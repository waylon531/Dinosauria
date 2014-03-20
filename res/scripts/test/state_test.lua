local CEGUISystem = CEGUI.System:getSingleton()
local CEGUIWindowManager = CEGUI.WindowManager:getSingleton()

class 'TestComponent' (engine.Component)

function TestComponent:__init()
   self.x = 0
   engine.Component.__init(self)
end

function TestComponent:getName()
   return "test"
end

class 'TestSystem' (engine.System)

function TestSystem:__init()
   engine.System.__init(self)
   self.mask = engine.EntityMask() + engine.getComponentID("test")
end

function TestSystem:run(pulse)
   --for id in self.entities do
   for i=0,self.entities:size()-1 do
      local id = self.entities:at(i)
      self.parent.mgr:getEntity(id):getComponent("test").x = 1
   end
end

class 'TestState' (engine.GameState)

function TestState:getName()
   return "test"
end

function TestState:__init()
   engine.GameState.__init(self)

   self.guiRoot = CEGUIWindowManager:loadWindowLayout("test.layout")

   self.world = engine.World()
   self.world:addSystem(TestSystem())

   local e = self.world:addEntity("test")
   e:addComponent(TestComponent())
   self.world:updateEntity("test")
end

function TestState:enter()
   CEGUISystem:setGUISheet(self.guiRoot)
end

function TestState:exit()
end

function TestState:pause()
   self:exit()
end

function TestState:resume()
   self:enter()
end

function TestState:onTick(pulse)
   self.world:onTick(0.0)
   print(self.world.mgr:getEntity("test"):getComponent("test").x)
end
