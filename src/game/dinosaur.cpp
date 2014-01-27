#include "game/dinosaur.hpp"

std::vector<std::shared_ptr<Dinosaur>> dinosaurs;

Dinosaur::Dinosaur(const std::string& fname)
{
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(fname.c_str());
  if(!result)
    {
      std::cout << "Error when parsing " << fname << std::endl;
      std::cout << "\t" << "Error description: " << result.description() << std::endl;
      std::cout << "\t" << "Error offset: " << result.offset << std::endl;
    }
  else
    {
      pugi::xml_node root = doc.child("dinosaur");
      name = root.attribute("name").value();
      speed = root.attribute("speed").as_float();
      description = root.child_value("description");
      material = std::shared_ptr<graphics::Material>(new graphics::Material(root.child_value("material")));
      bool isBin=false;
      if(root.child("mesh").attribute("bin") != NULL)
	{
	  isBin = true;
	}
      mesh = MKPTR(graphics::SkeletalMesh, root.child_value("mesh"), material, isBin);
    }
}

Dinosaur::~Dinosaur()
{
  mesh.reset();
  material.reset();
}

std::shared_ptr<Dinosaur> getDinosaur(const std::string& name)
{
  for(std::vector<std::shared_ptr<Dinosaur>>::iterator it=dinosaurs.begin(); it!=dinosaurs.end(); it++)
    {
      if((*it)->name == name) return (*it);
    }
  std::cout << "Could not find dinosaur with the name: " << name << " (returning first dinosaur)" << std::endl;
  return dinosaurs[0];
}

void Dinosaur::render()
{
  mesh->render();
}

DinosaurInstance::DinosaurInstance(std::shared_ptr<Dinosaur> dino) : parent(dino), pos(glm::vec3(0.f,0.f,0.f)), zrot(0.f)
{
  time = 0;
}

DinosaurInstance::DinosaurInstance(pugi::xml_node& node)
{
  time = 0;
  parent = getDinosaur(node.attribute("name").value());
  pos = parseVec3(node.attribute("pos").value());
  zrot = node.attribute("rot").as_float();
}

DinosaurInstance::~DinosaurInstance()
{
  parent.reset();
  matrix = glm::mat4();
}

void DinosaurInstance::save(pugi::xml_node& node)
{
  node.append_attribute("name") = parent->name.c_str();
  node.append_attribute("pos") = (asString(pos.x)+" "+asString(pos.y)+" "+asString(pos.z)).c_str();
  node.append_attribute("rot") = (asString(zrot)).c_str();
}

void DinosaurInstance::update(std::shared_ptr<Landscape> ground)
{
  pos.y = ground->eval(pos.x,pos.z);
  matrix = glm::translate(pos) * glm::rotate(zrot,0.f,1.f,0.f);
}

void DinosaurInstance::setMatrixView(glm::mat4* view)
{
  parent->mesh->setMatrixView(view);
}
void DinosaurInstance::setMatrixProject(glm::mat4* project)
{
  parent->mesh->setMatrixProject(project);
}
void DinosaurInstance::render()
{
  time += 0.01;
  if(time > 20.0) time = 0.0;
  parent->mesh->setTime(time);
  parent->mesh->m_model = matrix;
  parent->render();
}
