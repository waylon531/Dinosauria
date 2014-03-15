#include "engine/settings.hpp"

//custom external includes
#include <pugixml.hpp>
//custom includes
#include "util.hpp"

std::map<std::string, std::string> engine::userSettings;

void engine::loadSettings()
{
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(SETTINGS_FILENAME);
  if(!result)
    {
      //XML parsing error
      std::cout << "Error parsing XML file (" << SETTINGS_FILENAME << ":" << result.offset << ")" << std::endl << "\t" << result.description() << std::endl;
      return;
    }
  for(pugi::xml_node prop: doc.child("Settings").children())
    {
      std::cout << prop.attribute("Name").value() << std::endl;
      userSettings[prop.attribute("Name").value()] = prop.attribute("Value").value();
    }
}

void engine::saveSettings()
{
  pugi::xml_document doc;
  pugi::xml_node globalNode = doc.append_child("Settings");
  for(std::map<std::string, std::string>::iterator it=userSettings.begin(); it!=userSettings.end(); it++)
    {
      pugi::xml_node node = globalNode.append_child("Property");
      node.append_attribute("Name") = (*it).first.c_str();
      node.append_attribute("Value") = (*it).second.c_str();
    }
  doc.save_file(SETTINGS_FILENAME);
}
