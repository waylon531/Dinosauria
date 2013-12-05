#include "game/game.hpp"

#define CAMERA_POS glm::vec3(0.f,4.f,-5.f)
#define CAMERA_START glm::vec3(0.f,30.f,0.f)

Game::Game()
{
  graphics::u_time = std::shared_ptr<graphics::GLSLUniform>(new graphics::GLSLUniform("t",&time));
  camera = std::shared_ptr<graphics::Camera>(new graphics::Camera(CAMERA_START,
								  glm::vec3(0.f,0.f,0.f)));
  world = std::shared_ptr<graphics::World>(new graphics::World(camera));
  sky = std::shared_ptr<graphics::Sky>(new graphics::Sky(glm::vec3(.2f,.2f,.5f)));
  ground = std::shared_ptr<Landscape>(new Landscape);
  world->addMesh(ground);
  ocean = std::shared_ptr<Ocean>(new Ocean);
  
  fbo = MKPTR(graphics::GLFrameBuffer, W,H, true, "tex_depth");
  pass_color = MKPTR(graphics::GLPass, fbo, 0, "tex_color");
  fbo_Cwater = MKPTR(graphics::GLFrameBuffer, W,H, false);
  pass_Cwater_color = MKPTR(graphics::GLPass, fbo_Cwater, 0, "tex_color");
  pass_Cwater_depth = MKPTR(graphics::GLPass, fbo_Cwater, 1, "tex_depth");
  fbo->update();
  fbo_Cwater->update();

  shader_Cwater = MKPTR(graphics::GLSL, "res/shaders/fbo_pass.vert","res/effects/water.frag");
  shader_Cwater->attachUniform(pass_color->unif);
  shader_Cwater->attachUniform(fbo->unif_depth);
  shader_Cwater->attachUniform(ocean->fbo->unif_depth);
  shader_Cwater->attachUniform(ocean->pass_color->unif);
  shader_Cwater->attachUniform(ocean->pass_normal->unif);
  shader_Cwater->attachUniform(MKPTR(graphics::GLSLUniform,"eyeDir",&camera->look));
  shader_Cwater->attachUniform(MKPTR(graphics::GLSLUniform,"m_view",&camera->mat_view));
  shader_Cwater->attachUniform(MKPTR(graphics::GLSLUniform,"m_project",&camera->mat_project));
  comp_water = MKPTR(graphics::Compositor,shader_Cwater);
  
  shader_Cfog = std::shared_ptr<graphics::GLSL>(new graphics::GLSL("res/shaders/fbo_pass.vert","res/effects/fog.frag"));
  shader_Cfog->attachUniform(pass_Cwater_depth->unif);
  shader_Cfog->attachUniform(pass_Cwater_color->unif);
  comp_fog = MKPTR(graphics::Compositor,shader_Cfog);

  fbo_Cbloom_sub = MKPTR(graphics::GLFrameBuffer, W,H, false);
  fbo_Cbloom_hblur = MKPTR(graphics::GLFrameBuffer, W,H, false);
  fbo_Cbloom_vblur = MKPTR(graphics::GLFrameBuffer, W,H, false);
  pass_Cbloom_color_sub = MKPTR(graphics::GLPass, fbo_Cbloom_sub, 0, "tex_color");
  pass_Cbloom_color_hblur = MKPTR(graphics::GLPass, fbo_Cbloom_hblur, 0, "tex_color");
  pass_Cbloom_color_vblur = MKPTR(graphics::GLPass, fbo_Cbloom_vblur, 0, "tex_bloom");
  fbo_Cbloom_sub->update();
  fbo_Cbloom_hblur->update();
  fbo_Cbloom_vblur->update();
  shader_Cbloom_sub = MKPTR(graphics::GLSL,"res/shaders/fbo_pass.vert","res/effects/bloom/sub.frag");
  shader_Cbloom_sub->attachUniform(pass_Cwater_color->unif);
  shader_Cbloom_hblur = MKPTR(graphics::GLSL,"res/shaders/fbo_pass.vert","res/effects/bloom/hblur.frag");
  shader_Cbloom_hblur->attachUniform(pass_Cbloom_color_sub->unif);
  shader_Cbloom_vblur = MKPTR(graphics::GLSL,"res/shaders/fbo_pass.vert","res/effects/bloom/vblur.frag");
  shader_Cbloom_vblur->attachUniform(pass_Cbloom_color_hblur->unif);
  shader_Cbloom_final = MKPTR(graphics::GLSL,"res/shaders/fbo_pass.vert","res/effects/bloom/final.frag");
  shader_Cbloom_final->attachUniform(pass_Cbloom_color_vblur->unif);
  shader_Cbloom_final->attachUniform(pass_Cwater_color->unif);
  comp_bloom = MKPTR(graphics::Compositor,std::shared_ptr<graphics::GLSL>(NULL));
  
  std::shared_ptr<graphics::GLSL> shader_Cbasic = MKPTR(graphics::GLSL,"res/shaders/fbo_pass.vert","res/effects/basic.frag");
  shader_Cbasic->attachUniform(pass_color->unif);
  comp_basic = MKPTR(graphics::Compositor,shader_Cbasic);
}

#include <pugixml.hpp>
#include <sys/stat.h>

Game::~Game()
{
  camera.reset();
  world.reset();
  player.reset();
  ground.reset();
}

void Game::load(const std::string& dir, float* value, float* value2, Callback& callback)
{
#define NUM_TASKS 2
  const float inc = 1.f/NUM_TASKS;
  *value=0.f;
  callback.call();
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file((dir+"game.xml").c_str());
  if(!result)
    {
      std::cout << "Error when parsing " << dir+"game.xml" << std::endl;
      std::cout << "\t" << "Error description: " << result.description() << std::endl;
      std::cout << "\t" << "Error offset: " << result.offset << std::endl;
    }
  else
    {
      pugi::xml_node player_node = doc.child("player");
      player = std::shared_ptr<DinosaurInstance>(new DinosaurInstance(player_node));
      world->addMesh(player);
      pugi::xml_node game_node = doc.child("game");
      time = game_node.attribute("time").as_int();
    }
  *value+=inc;
  callback.call();
  ground->reset();
  ground->loadMap(dir, value2,callback);
  *value+=inc;
  callback.call();
#undef NUM_TASKS
}

void Game::start(float* value, float* value2, Callback& callback)
{
#define NUM_TASKS 1
  time = 0;
  const float inc = 1.f/NUM_TASKS;
  *value=0.f;
  ground->reset();
  ground->generate(value2, callback);
  *value+=inc;
#undef NUM_TASKS
}

glm::vec3 Game::getCameraLoc()
{
  return player->pos + glm::vec3(player->matrix * glm::vec4(CAMERA_POS,0.f));
}

void Game::save(const std::string& name)
{
  std::string dir = "saves/"+name+"/";
  mkdir(dir.c_str(),0777);
  pugi::xml_document doc;
  pugi::xml_node node_player = doc.append_child("player");
  player->save(node_player);
  pugi::xml_node node_game = doc.append_child("game");
  node_game.append_attribute("time") = asString(time).c_str();
  doc.save_file((dir+"game.xml").c_str());

  ground->save(dir);
}

void Game::setPlayer(std::shared_ptr<Dinosaur> dino)
{
  player = std::shared_ptr<DinosaurInstance>(new DinosaurInstance(dino));
  world->addMesh(player);
  player->pos = glm::vec3(0.f,0.f,0.f);
}

void Game::render()
{
#define ALPHA 0.025f
  camera->setPos(camera->pos*(1.0f-ALPHA) + getCameraLoc()*ALPHA);
  float cmax = ground->eval(camera->pos.x, camera->pos.z) + 2.0;
  if(camera->pos.y <= cmax) camera->setPos(glm::vec3(camera->pos.x, cmax, camera->pos.z));
#undef ALPHA
  time++;
  float angle = (float)time/1000.0f;
  float cosa = cos(angle);
  world->sun = glm::normalize(glm::vec3(cosa*.25f,sin(angle),cosa*.75f));
  //world->sun = glm::normalize(glm::vec3(1.01f,1.01f,1.01f));

  fbo->use();
  pass_color->use();
  sky->render();
  world->render();
  fbo->unuse();
  pass_color->useTex();

  world->use(ocean);
  ocean->render(world);

  //compositing
  ocean->pass_color->useTex();
  ocean->pass_normal->useTex();
  ocean->fbo->useTex();
  fbo->useTex();
  fbo_Cwater->use();
  pass_Cwater_color->use();
  pass_Cwater_depth->use();
  comp_water->draw();
  fbo_Cwater->unuse();  
  fbo_Cwater->useTex();
  pass_Cwater_color->useTex();
  pass_Cwater_depth->useTex();

  //bloom
  comp_bloom->shader = shader_Cbloom_sub;
  fbo_Cbloom_sub->use();
  pass_Cbloom_color_sub->use();
  comp_bloom->draw();
  fbo_Cbloom_sub->unuse();
  pass_Cbloom_color_sub->useTex();
  fbo_Cbloom_sub->useTex();
  
  fbo_Cbloom_hblur->use();
  comp_bloom->shader = shader_Cbloom_hblur;
  pass_Cbloom_color_hblur->use();
  comp_bloom->draw();
  fbo_Cbloom_hblur->unuse();
  pass_Cbloom_color_hblur->useTex();
  fbo_Cbloom_hblur->useTex();
  
  fbo_Cbloom_vblur->use();
  comp_bloom->shader = shader_Cbloom_vblur;
  pass_Cbloom_color_vblur->use();
  comp_bloom->draw();
  fbo_Cbloom_vblur->unuse();
  pass_Cbloom_color_vblur->useTex();
  fbo_Cbloom_vblur->useTex();
  
  comp_bloom->shader = shader_Cbloom_final;
  comp_bloom->draw();
  //comp_final->draw();
}

void Game::resetGame()
{
  camera->setPos(CAMERA_START);
  world->removeMesh(player);
  player.reset();
}

void Game::onMouseOver(const glm::vec2& pos)
{
}
void Game::onNotMouseOver()
{
}
void Game::onMouseClick(const glm::vec2& pos)
{
}
void Game::onMouseRelease(const glm::vec2& pos) 
{
}
void Game::onKey(GLFWwindow* window)
{
  if(glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_PRESS)
    {
      player->zrot += 1.f;
    }
  if(glfwGetKey(window,GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
      player->zrot -= 1.f;
    }
  if(glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS)
    {
      player->pos += glm::vec3(player->matrix * glm::vec4(.0f,.0f,player->parent->speed,0.f));
    }
  if(glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS)
    {
      player->pos -= glm::vec3(player->matrix * glm::vec4(.0f,.0f,player->parent->speed,0.f));
    }
  if(glfwGetKeyOnce(window,GLFW_KEY_Z) == GLFW_PRESS)
    {
      ground->isWireframe = 1-ground->isWireframe;
    }
  player->update(ground);
  camera->setLook(player->pos);
}
