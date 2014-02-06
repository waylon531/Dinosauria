#include "game/game.hpp"

#define CAMERA_POS glm::vec3(0.f,4.f,-5.f)
#define CAMERA_START glm::vec3(0.f,30.f,0.f)

#define NUM_ROCKS 64

Game::Game()
{
  graphics::u_time = std::shared_ptr<graphics::GLSLUniform>(new graphics::GLSLUniform("t",&time));
  camera = std::shared_ptr<graphics::Camera>(new graphics::Camera(CAMERA_START,
								  glm::vec3(0.f,0.f,0.f)));
  camScale = 1.0f;
  debugMode = false;
  
  world = std::shared_ptr<graphics::World>(new graphics::World(camera));
  sky = std::shared_ptr<graphics::Sky>(new graphics::Sky(glm::vec3(.2f,.2f,.5f)));
  ground = std::shared_ptr<Landscape>(new Landscape);
  world->addMesh(ground);
  ocean = std::shared_ptr<Ocean>(new Ocean);

  init_rocks();

  physicsWorld = std::shared_ptr<physics::World>(new physics::World);
  physicsWorld->world->setDebugDrawer(new physics::DebugDraw);
  
  fbo = MKPTR(graphics::GLFrameBuffer, W,H, true, "tex_depth");
  pass_color = MKPTR(graphics::GLPass, fbo, 0, "tex_color", GL_RGBA32F, GL_HALF_FLOAT);
  fbo_reflection = MKPTR(graphics::GLFrameBuffer, W,H, true, "tex_depth_reverse");
  pass_color_reflection = MKPTR(graphics::GLPass, fbo_reflection, 0, "tex_color_reverse", GL_RGBA32F, GL_HALF_FLOAT);
  fbo_reflection->update();
  fbo_Cwater = MKPTR(graphics::GLFrameBuffer, W,H, false);
  pass_Cwater_color = MKPTR(graphics::GLPass, fbo_Cwater, 0, "tex_color", GL_RGBA32F, GL_HALF_FLOAT);
  pass_Cwater_depth = MKPTR(graphics::GLPass, fbo_Cwater, 1, "tex_depth");
  fbo->update();
  fbo_Cwater->update();

  shader_Cwater = MKPTR(graphics::GLSL, "res/shaders/fbo_pass.vert","res/effects/water_comp.frag");
  shader_Cwater->attachUniform(pass_color->unif);
  shader_Cwater->attachUniform(fbo_reflection->unif_depth);
  shader_Cwater->attachUniform(pass_color_reflection->unif);
  shader_Cwater->attachUniform(world->fbo->unif_depth);
  shader_Cwater->attachUniform(ocean->fbo->unif_depth);
  shader_Cwater->attachUniform(ocean->pass_color->unif);
  shader_Cwater->attachUniform(ocean->pass_normal->unif);
  shader_Cwater->attachUniform(MKPTR(graphics::GLSLUniform,"eyeDir",&camera->look));
  shader_Cwater->attachUniform(MKPTR(graphics::GLSLUniform,"eyePos",&camera->pos));
  shader_Cwater->attachUniform(MKPTR(graphics::GLSLUniform,"m_view",&camera->mat_view));
  shader_Cwater->attachUniform(MKPTR(graphics::GLSLUniform,"m_project",&camera->mat_project));
  comp_water = MKPTR(graphics::Compositor,shader_Cwater);
  
  shader_Cfog = std::shared_ptr<graphics::GLSL>(new graphics::GLSL("res/shaders/fbo_pass.vert","res/effects/fog.frag"));
  shader_Cfog->attachUniform(pass_Cwater_depth->unif);
  shader_Cfog->attachUniform(pass_Cwater_color->unif);
  comp_fog = MKPTR(graphics::Compositor,shader_Cfog);

#define BLOOM_W (W/4)
#define BLOOM_H (H/4)
  fbo_Cbloom_sub = MKPTR(graphics::GLFrameBuffer, BLOOM_W,BLOOM_H, false);
  fbo_Cbloom_hblur = MKPTR(graphics::GLFrameBuffer, W,H, false);
  fbo_Cbloom_vblur = MKPTR(graphics::GLFrameBuffer, W,H, false);
  pass_Cbloom_color_sub = MKPTR(graphics::GLPass, fbo_Cbloom_sub, 0, "tex_color",GL_RGBA32F,GL_HALF_FLOAT);
  pass_Cbloom_color_hblur = MKPTR(graphics::GLPass, fbo_Cbloom_hblur, 0, "tex_color",GL_RGBA32F,GL_HALF_FLOAT);
  pass_Cbloom_color_vblur = MKPTR(graphics::GLPass, fbo_Cbloom_vblur, 0, "tex_bloom",GL_RGBA32F,GL_HALF_FLOAT);
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

  fbo_Cbloom_final = MKPTR(graphics::GLFrameBuffer, W,H, false);
  pass_Cbloom_color_final = MKPTR(graphics::GLPass, fbo_Cbloom_final, 0, "tex_color");
  fbo_Cbloom_final->update();
  fbo_Cdof_down = MKPTR(graphics::GLFrameBuffer, W/2, H/2, false);
  fbo_Cdof_hblur = MKPTR(graphics::GLFrameBuffer, W,H, false);
  fbo_Cdof_vblur = MKPTR(graphics::GLFrameBuffer, W,H, false);
  pass_Cdof_color_down = MKPTR(graphics::GLPass, fbo_Cdof_down, 0, "tex_color");
  pass_Cdof_color_hblur = MKPTR(graphics::GLPass, fbo_Cdof_hblur, 0, "tex_color");
  pass_Cdof_color_vblur = MKPTR(graphics::GLPass, fbo_Cdof_vblur, 0, "tex_dof");
  fbo_Cdof_down->update();
  fbo_Cdof_hblur->update();
  fbo_Cdof_vblur->update();
  shader_Cdof_down = MKPTR(graphics::GLSL,"res/shaders/fbo_pass.vert","res/effects/dof/down.frag");
  shader_Cdof_down->attachUniform(pass_Cwater_color->unif);
  shader_Cdof_hblur = MKPTR(graphics::GLSL,"res/shaders/fbo_pass.vert","res/effects/dof/hblur.frag");
  shader_Cdof_hblur->attachUniform(pass_Cdof_color_down->unif);
  shader_Cdof_vblur = MKPTR(graphics::GLSL,"res/shaders/fbo_pass.vert","res/effects/dof/vblur.frag");
  shader_Cdof_vblur->attachUniform(pass_Cdof_color_hblur->unif);
  shader_Cdof_final = MKPTR(graphics::GLSL,"res/shaders/fbo_pass.vert","res/effects/dof/dof.frag");
  shader_Cdof_final->attachUniform(pass_Cdof_color_vblur->unif);
  shader_Cdof_final->attachUniform(pass_Cbloom_color_final->unif);
  shader_Cdof_final->attachUniform(world->fbo->unif_depth);
  shader_Cdof_final->attachUniform(MKPTR(graphics::GLSLUniform,"focalLength",&focalLength));
  
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
      physicsWorld->addBody(player->body);
      pugi::xml_node game_node = doc.child("game");
      time = game_node.attribute("time").as_int();
    }

  *value+=inc;
  callback.call();
  ground->reset();
  ground->loadMap(dir, value2,callback);
  physicsWorld->addBody(ground->body);

  for(int i=0; i<NUM_ROCKS; i++)
    {
      std::shared_ptr<Rock> rock(new Rock(ground));
      world->addMesh(rock);
      physicsWorld->addBody(rock->body);
      rocks.push_back(rock);
    }
  
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
  for(int i=0; i<NUM_ROCKS; i++)
    {
      std::shared_ptr<Rock> rock(new Rock(ground));
      world->addMesh(rock);
      physicsWorld->addBody(rock->body);
      rocks.push_back(rock);
    }
  physicsWorld->addBody(ground->body);
  *value+=inc;
#undef NUM_TASKS
}

glm::vec3 Game::getCameraLoc()
{
  return player->pos + glm::vec3(player->matrix * glm::vec4(camScale*CAMERA_POS,0.f));
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
  physicsWorld->addBody(player->body);
  player->pos = glm::vec3(0.f,0.f,0.f);
}

void Game::render()
{
  physicsWorld->step();
  physicsWorld->step();
  physicsWorld->step();
  physicsWorld->step();
  physicsWorld->step();
  physicsWorld->step();
  
#define ALPHA 0.025f
  glm::vec3 camLoc = getCameraLoc();
  camera->setPos(camera->pos*(1.0f-ALPHA) + camLoc*ALPHA);
  float cmax = ground->eval(camera->pos.x, camera->pos.z) + 2.0;
  if(camera->pos.y <= cmax) camera->setPos(glm::vec3(camera->pos.x, cmax, camera->pos.z));
  focalLength = glm::distance(camera->pos , player->pos);
  *ground->m_tessView = camera->mat_view;
#undef ALPHA
  time++;
  float angle = (float)time/1000.0f;
  float cosa = cos(angle);
  world->sun = glm::normalize(glm::vec3(cosa*.25f,sin(angle),cosa*.75f));
  //world->sun = glm::normalize(glm::vec3(1.01f,1.01f,1.01f));

  sky->render();
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf((const GLfloat*)&camera->mat_project[0][0]);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf((const GLfloat*)&camera->mat_view[0][0]);

  if(debugMode)
    {
      physicsWorld->world->debugDrawWorld();
      ((physics::DebugDraw*)physicsWorld->world->getDebugDrawer())->renderFrame();
      ((physics::DebugDraw*)physicsWorld->world->getDebugDrawer())->startFrame();
    }
  else
    {
      fbo_reflection->use();
      pass_color_reflection->use();
      world->isWaterPass = 1;
      camera->mat_view = camera->mat_view * glm::scale(1.f,-1.f,1.f);
      sky->render();
      world->render();
      fbo_reflection->unuse();
      pass_color_reflection->useTex();
      camera->mat_view = camera->mat_view * glm::scale(1.f,-1.f,1.f);
      world->isWaterPass = 0;
      
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
      fbo_Cbloom_final->use();
      comp_bloom->draw();
      fbo_Cbloom_final->unuse();
      pass_Cbloom_color_final->useTex();
      
      comp_bloom->shader = shader_Cdof_down;
      fbo_Cdof_down->use();
      comp_bloom->draw();
      fbo_Cdof_down->unuse();
      pass_Cdof_color_down->useTex();
      
      comp_bloom->shader = shader_Cdof_hblur;
      fbo_Cdof_hblur->use();
      comp_bloom->draw();
      fbo_Cdof_hblur->unuse();
      pass_Cdof_color_hblur->useTex();
      
      comp_bloom->shader = shader_Cdof_vblur;
      fbo_Cdof_vblur->use();
      comp_bloom->draw();
      fbo_Cdof_vblur->unuse();
      pass_Cdof_color_vblur->useTex();
      
      comp_bloom->shader = shader_Cdof_final;
      comp_bloom->draw();
    }
}

void Game::resetGame()
{
  camera->setPos(CAMERA_START);
  world->removeMesh(player);
  physicsWorld->removeBody(player->body);
  physicsWorld->removeBody(ground->body);
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
#define ROT_INC 5.f
  if(glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_PRESS)
    {
      player->rot.y += ROT_INC;
    }
  if(glfwGetKey(window,GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
      player->rot.y -= ROT_INC;
    }
  if(glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS)
    {
      player->body->impulseTranslate(glm::vec3(player->matrix * glm::vec4(.0f,.0f,player->parent->speed,0.f)));
    }
  if(glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS)
    {
      player->body->impulseTranslate(-glm::vec3(player->matrix * glm::vec4(.0f,.0f,player->parent->speed,0.f)));
    }
  if(glfwGetKeyOnce(window,GLFW_KEY_SPACE) == GLFW_PRESS)
    {
      player->body->impulseTranslate(glm::vec3(player->matrix * glm::vec4(.0f,10.f,0.f,0.f)));
    }
  if(glfwGetKeyOnce(window,GLFW_KEY_Z) == GLFW_PRESS)
    {
      ground->isWireframe = 1-ground->isWireframe;
    }
  if(glfwGetKeyOnce(window,GLFW_KEY_D) == GLFW_PRESS)
    {
      debugMode = !debugMode;
    }
  #define SCALE_INC 1.05
  if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
    {
      camScale *= SCALE_INC;
    }
  if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
    {
      camScale /= SCALE_INC;
    }
  player->update(ground);
  camera->setLook(player->pos);
}
