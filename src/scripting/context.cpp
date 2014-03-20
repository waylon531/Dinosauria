#include "scripting/context.hpp"

//STL includes
#include <sstream>
//external includes
#include <ScriptingModules/LuaScriptModule/CEGUILua.h>
#include <CEGUISystem.h>
//custom external includes
extern "C"
{
  #include "lua.h"
  #include "lualib.h"
};
#include "luabind/luabind.hpp"
#include "luabind/class_info.hpp"
#include "pugixml.hpp"

int addFileAndLineInfo(lua_State* L)
{
  lua_Debug d;
  lua_getstack(L, 1, &d);
  lua_getinfo(L, "Sln", &d);
  std::string err = lua_tostring(L, -1);
  lua_pop(L, 1);
  std::stringstream msg;
  msg << d.short_src << ":" << d.currentline;

  if (d.name != 0)
    {
      msg << "(" << d.namewhat << " " << d.name << ")";
    }
  msg << " " << err;
  lua_pushstring(L, msg.str().c_str());
  return 1;
}

void engine::LuaEngine::startup(const bool cegui)
{
  state = luaL_newstate();
  luaL_openlibs(state);
  if(cegui)
    {
      CEGUI::LuaScriptModule* scriptModule = &CEGUI::LuaScriptModule::create(state);
      scriptModule->createBindings();
      CEGUI::System::getSingleton().setScriptingModule(scriptModule);
      state = scriptModule->getLuaState();
    }
  //luaL_openlibs(state);
  luabind::open(state);
  luabind::bind_class_info(state);

  luabind::set_pcall_callback(addFileAndLineInfo);
}

void engine::LuaEngine::shutdown()
{
  lua_close(state);
}

void engine::LuaEngine::loadScripts(const boost::filesystem::path& dir)
{
  boost::filesystem::path manifestPath = dir / "scripts.xml";
  if(!boost::filesystem::exists(manifestPath))
    {
      std::cout << "Could not find scripts.xml in directory " << dir << " (not loading Lua scripts)" << std::endl;
      return;
    }
  //parse scripts.xml
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(manifestPath.string().c_str());
  if(!result)
    {
      //XML parsing error
      std::cout << "Error parsing XML file (" << manifestPath << ":" << result.offset << ")" << std::endl << "\t" << result.description() << std::endl;
      return;
    }
  for(pugi::xml_node node: doc.child("Scripts").children())
    {
      if(std::string(node.name()) == "Directory")
	{
	  loadScripts(dir / boost::filesystem::path(node.attribute("Name").value()));
	}
      else if(std::string(node.name()) == "Script")
	{
	  int error = 0;
	  std::cout << "Loading file " << (dir / std::string(node.attribute("Name").value())) << std::endl;
	  error = luaL_loadfile(state, (dir / node.attribute("Name").value()).string().c_str());
	  error = error || luabind::detail::pcall(state, 0, LUA_MULTRET);
	  if(error)
	    {
	      std::string msg = lua_tostring(state, -1);
	      lua_pop(state, 1);
	      std::cout << "Aborting load in this directory: " << msg << std::endl;
	      return;
	    }
	}
      else
	{
	  std::cout << "Unknown identifier " << node.name() << ": aborting load in this directory" << std::endl;
	  return;
	}
    }
}
