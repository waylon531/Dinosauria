//file: engine/settings.hpp
//purpose: user settings storage

//STL includes
#include <map>

namespace engine
{

  /** The user settings map */
  extern std::map<std::string, std::string> userSettings;

  /** Save all of the user settings to a file */
  extern void saveSettings();

  /** Load all of the user settings from a file */
  extern void loadSettings();
};
