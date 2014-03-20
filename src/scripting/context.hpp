//file: scripting/context.hpp
//purpose: Lua scripting context startup and shutdown support

#pragma once

//external includes
#include "boost/filesystem.hpp"

struct lua_State;

namespace engine
{
  /** A Lua context */
  class LuaEngine
  {
  public:

    /** Get the singleton instance of this class
     * @return singleton instance */
    static inline LuaEngine& getSingleton()
    {
      static LuaEngine engine;
      return engine;
    }

    /** The lua state used in the engine */
    lua_State* state;

    /** Startup the lua context
     * @param cegui whether to use CEGUI linkage */
    void startup(const bool cegui=true);

    /** Destroy the lua context */
    void shutdown();

    /** Load a directory with a Lua scripts.xml in it containing all of the scripts to be loaded
     * @param dir directory to load from */
    void loadScripts(const boost::filesystem::path& dir);

    /** Load all of the Lua scripts */
    inline void loadAllScripts()
    {
      loadScripts(boost::filesystem::path("res/scripts"));
    }
  };
};
