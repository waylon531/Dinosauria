#include "graphics/graphics.hpp"
#include "gui/gui.hpp"
#include "game/game.hpp"

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <pugixml.hpp>
#include <pthread.h>

bool running;

class NewGameMenu;
class MainMenu;
class GameFrame;
class GenFrame;
class LoadFrame;
std::shared_ptr<gui::Frame> current_frame;
std::shared_ptr<NewGameMenu> frame_newgame;
std::shared_ptr<MainMenu> frame_mainmenu;
std::shared_ptr<GameFrame> frame_game;
std::shared_ptr<GenFrame> frame_gen;
std::shared_ptr<LoadFrame> frame_load;
std::shared_ptr<Game> game;

graphics::GLContext* context;

class GenFrame: public gui::Frame
{
private:

  std::shared_ptr<graphics::Sky> sky;
  std::shared_ptr<gui::StaticText> title;
  //std::shared_ptr<gui::DynamicText> status;
  std::shared_ptr<gui::ProgressBar> bar0;
  std::shared_ptr<gui::ProgressBar> bar1;
  
public:

  GenFrame() : Frame(0,0,W,H)
  {
    sky = std::shared_ptr<graphics::Sky>(new graphics::Sky(glm::vec3(.7f,.7f,.7f)));
    view->addObject(sky);
    title = std::shared_ptr<gui::StaticText>(new gui::StaticText("Generating World...", glm::vec4(1.f,0.0,0.0,1.0), glm::vec2(400,900), gui::font_150pt));
    gui->addWidget(title);
    bar0 = std::shared_ptr<gui::ProgressBar>(new gui::ProgressBar(glm::vec2(100,300),850));
    gui->addWidget(bar0);
    bar1 = std::shared_ptr<gui::ProgressBar>(new gui::ProgressBar(glm::vec2(100,200),850));
    gui->addWidget(bar1);
  }
  
  ~GenFrame()
  {
    sky.reset();
    title.reset();
    bar0.reset();
    bar1.reset();
  }

  void call_render()
  {
    render();
    context->flip();
  }
  static void _call_render(void* _this)
  {
    ((GenFrame*)_this)->call_render();
  }

  void start()
  {
    bar0->value = .0f;
    bar0->update();
    bar1->value = .0f;
    bar1->update();
    render();
    Callback c((void*)this, _call_render);
    game->start(&(bar0->value), &(bar1->value), c);
    current_frame = frame_game;
  }
  
  void render()
  {
    bar0->update();
    bar1->update();
    _render();
    context->flip();
  }
};

class LoadFrame: public gui::Frame
{
private:

  std::shared_ptr<graphics::Sky> sky;
  std::shared_ptr<gui::StaticText> title;
  //std::shared_ptr<gui::DynamicText> status;
  std::shared_ptr<gui::ProgressBar> bar0;
  std::shared_ptr<gui::ProgressBar> bar1;
  
public:

  LoadFrame() : Frame(0,0,W,H)
  {
    sky = std::shared_ptr<graphics::Sky>(new graphics::Sky(glm::vec3(.7f,.7f,.7f)));
    view->addObject(sky);
    title = std::shared_ptr<gui::StaticText>(new gui::StaticText("Loading Game...", glm::vec4(1.f,0.0,0.0,1.0), glm::vec2(400,900), gui::font_150pt));
    gui->addWidget(title);
    bar0 = std::shared_ptr<gui::ProgressBar>(new gui::ProgressBar(glm::vec2(100,300),850));
    gui->addWidget(bar0);
    bar1 = std::shared_ptr<gui::ProgressBar>(new gui::ProgressBar(glm::vec2(100,200),850));
    gui->addWidget(bar1);
  }
  
  ~LoadFrame()
  {
    sky.reset();
    title.reset();
    bar0.reset();
    bar1.reset();
  }

  void call_render()
  {
    render();
    context->flip();
  }
  static void _call_render(void* _this)
  {
    ((GenFrame*)_this)->call_render();
  }

  void start()
  {
    bar0->value = .0f;
    bar0->update();
    bar1->value = .0f;
    bar1->update();
    render();
    Callback c((void*)this, _call_render);

    //game->load("saves/s0/");
    game->load("saves/s0/", &(bar0->value), &(bar1->value), c);
    current_frame = frame_game;
  }
  
  void render()
  {
    bar0->update();
    bar1->update();
    _render();
    context->flip();
  }
};

class NewGameMenu: public gui::Frame
{
private:

  std::shared_ptr<gui::StaticText> title;
  std::shared_ptr<gui::Button> button_back;
  std::shared_ptr<gui::Button> button_start;
  std::shared_ptr<graphics::Sky> sky;
  int time;
  std::shared_ptr<graphics::GLSLUniform> u_time;

  class DinoFrame;
  DinoFrame* current_dino;

  class DinoFrame: public gui::Frame
  {
  private:

    std::shared_ptr<gui::StaticText> name;
    int* Time;
    std::shared_ptr<graphics::Sky> sky;
    DinoFrame** cur_dino;
    std::shared_ptr<graphics::World> world;

  public:

    std::shared_ptr<Dinosaur> dino;
    std::shared_ptr<gui::StaticText> description;

    DinoFrame(std::shared_ptr<Dinosaur> Dino, const int x, const int y, DinoFrame** cdino, int* _time) : Frame(x,y,256,256), cur_dino(cdino), Time(_time), dino(Dino)
    {
      world = std::shared_ptr<graphics::World>(new graphics::World(std::shared_ptr<graphics::Camera>(new graphics::Camera(glm::vec3(-5.f,10.f,-5.f),
															  glm::vec3(0.f,0.f,0.f),
															  1.f))));
      sky = std::shared_ptr<graphics::Sky>(new graphics::Sky(glm::vec3(0.5f,0.6f,0.5f)));
      name = std::shared_ptr<gui::StaticText>(new gui::StaticText(dino->name, glm::vec4(0.f,0.f,0.f,1.f), glm::vec2(10,10), gui::font_50pt, view->unif_ortho));
      description = std::shared_ptr<gui::StaticText>(new gui::StaticText("Description: "+dino->description, glm::vec4(0.f,0.f,0.f,1.f), glm::vec2(50,400), gui::font_50pt));
      view->addObject(sky);
      gui->addWidget(name);
      world->addMesh(dino->mesh);
      view->addObject(world);
      *cur_dino = this;
    }
    ~DinoFrame()
    {
      dino.reset();
      name.reset();
      description.reset();
      sky.reset();
      world.reset();
    }
    void render()
    {
      if(*cur_dino == this)
	{
	  sky->color = glm::vec3(0.7f,0.7f,0.5f);
	}
      dino->mesh->m_model = glm::rotate(*Time*.5f, 0.f,1.f,0.f) * glm::scale(glm::vec3(1.f,1.f,1.f)*2.5f);
      _render();
    }

    void onMouseOver(const glm::vec2& pos)
    {
      sky->color = glm::vec3(0.5f,0.7f,0.5f);
    }
    void onNotMouseOver()
    {
      sky->color = glm::vec3(0.5f,0.6f,0.5f);
    }
    void onMouseClick(const glm::vec2& pos)
    {
      (*cur_dino)->e_mouseOver = not (*cur_dino)->e_mouseOver;
      *cur_dino = this;
    }
  };
  
public:

  void call_back()
  {
    game->resetGame();
    current_frame = frame_mainmenu;
  }
  void call_start()
  {
    game->setPlayer(current_dino->dino);
    current_frame = frame_gen;
    frame_gen->start();
  }
  static void _call_back(void* _this)
  {
    ((NewGameMenu*)_this)->call_back();
  }
  static void _call_start(void* _this)
  {
    ((NewGameMenu*)_this)->call_start();
  }

  NewGameMenu() : Frame(0,0,W,H)
  {
    game = std::shared_ptr<Game>(new Game);
    time = 0;
    u_time = std::shared_ptr<graphics::GLSLUniform>(new graphics::GLSLUniform("t",&time));
    sky = std::shared_ptr<graphics::Sky>(new graphics::Sky(glm::vec3(0.5,0.5,0.6)));
    view->addObject(sky);
    title = std::shared_ptr<gui::StaticText>(new gui::StaticText("New Game", glm::vec4(1.0,0.0,0.0,1.0), glm::vec2(450,900), gui::font_150pt));
    gui->addWidget(title);
    button_back = std::shared_ptr<gui::Button>(new gui::Button("Back", glm::vec2(50,900), Callback(this, _call_back)));
    gui->addWidget(button_back);
    button_start = std::shared_ptr<gui::Button>(new gui::Button("Start", glm::vec2(600,50), Callback(this, _call_start)));
    gui->addWidget(button_start);
    addSubframe(std::shared_ptr<DinoFrame>(new DinoFrame(dinosaurs[0],100,500, &current_dino, &time)));
    //addSubframe(std::shared_ptr<DinoFrame>(new DinoFrame(dinosaurs[1],400,500, &current_dino, &time)));
    //addSubframe(std::shared_ptr<DinoFrame>(new DinoFrame(dinosaurs[2],700,500, &current_dino, &time)));
  }
  ~NewGameMenu()
  {
    sky.reset();
    title.reset();
    button_back.reset();
    u_time.reset();
  }

  void render()
  {
    time++;
    _render();
    view->use();
    current_dino->description->render();
  }
};

class MainMenu: public gui::Frame
{
private:

  std::shared_ptr<graphics::Background> background;
  std::shared_ptr<gui::StaticText> title;
  std::shared_ptr<gui::Button> button_newGame;
  std::shared_ptr<gui::Button> button_loadGame;
  std::shared_ptr<gui::Button> button_quit;

public:
  
  void call_startGame()
  {
    current_frame = frame_newgame;
  }
  void call_loadGame()
  {
    current_frame = frame_game;
    frame_load->start();
  }
  void call_exit()
  {
    running=false;
  }

  static void _call_startGame(void* _this)
  {
    ((MainMenu*)_this)->call_startGame();
  }
  static void _call_loadGame(void* _this)
  {
    ((MainMenu*)_this)->call_loadGame();
  }
  static void _call_exit(void* _this)
  {
    ((MainMenu*)_this)->call_exit();
  }

  MainMenu() : Frame(0,0,W,H)
  {
    background = std::shared_ptr<graphics::Background>(new graphics::Background("res/images/back_2.png"));
    view->addObject(background);
    title = std::shared_ptr<gui::StaticText>(new gui::StaticText("Dinosauria", glm::vec4(1.f,0.f,0.f,1.f), glm::vec2(250,800), gui::font_200pt));
    gui->addWidget(title);
    button_newGame = std::shared_ptr<gui::Button>(new gui::Button("New Game", glm::vec2(450,700), Callback(this, _call_startGame)));
    gui->addWidget(button_newGame);
    button_loadGame = std::shared_ptr<gui::Button>(new gui::Button("Load Game", glm::vec2(450,600), Callback(this, _call_loadGame)));
    gui->addWidget(button_loadGame);
    button_quit = std::shared_ptr<gui::Button>(new gui::Button("Exit", glm::vec2(450,500), Callback(this, _call_exit)));
    gui->addWidget(button_quit);
  }
  ~MainMenu()
  {
    background.reset();
    title.reset();
    button_newGame.reset();
    button_loadGame.reset();
    button_quit.reset();
  }

  void render()
  {
    _render();
  }
};

class GameFrame: public gui::Frame
{
private:

  std::shared_ptr<gui::Button> button_mmenu;
  std::shared_ptr<gui::Button> button_save;
  
public:

  void call_mmenu()
  {
    game->resetGame();
    current_frame = frame_mainmenu;
  }
  void call_save()
  {
    game->save("s0");
  }
  static void _call_mmenu(void* _this)
  {
    ((GameFrame*)_this)->call_mmenu();
  }
  static void _call_save(void* _this)
  {
    ((GameFrame*)_this)->call_save();
  }

  GameFrame() : Frame(0,0,W,H)
  {
    button_mmenu = std::shared_ptr<gui::Button>(new gui::Button("Main Menu", glm::vec2(50,50), Callback(this, _call_mmenu)));
    gui->addWidget(button_mmenu);
    button_save = std::shared_ptr<gui::Button>(new gui::Button("Save", glm::vec2(250,50), Callback(this, _call_save)));
    gui->addWidget(button_save);
  }
  
  ~GameFrame()
  {
  }

  void render()
  {
    game->render();
    _render();
  }

  void onMouseOver(const glm::vec2& pos)
  {
    game->onMouseOver(pos);
  }
  void onNotMouseOver()
  {
    game->onNotMouseOver();
  }
  void onMouseClick(const glm::vec2& pos)
  {
    game->onMouseClick(pos);
  }
  void onMouseRelease(const glm::vec2& pos)
  {
    game->onMouseRelease(pos);
  }
  void onKey(GLFWwindow* window)
  {
    game->onKey(window);
  }
};

  

void initAllDinos()
{
  //dinosaurs.push_back(std::shared_ptr<Dinosaur>(new Dinosaur("res/dinos/triceratops.xml")));
  dinosaurs.push_back(std::shared_ptr<Dinosaur>(new Dinosaur("res/dinos/deinonychus.xml")));
  //dinosaurs.push_back(std::shared_ptr<Dinosaur>(new Dinosaur("res/dinos/parasaur.xml")));
}

#include <GL/glu.h>

int main(int argc, char** argv)
{
  //run primary prototype
  context = new graphics::GLContext(REAL_W,REAL_H,"Dinosauria");
  graphics::initAtlas();
  gui::initFonts();
  initAllDinos();
  frame_mainmenu = std::shared_ptr<MainMenu>(new MainMenu);
  frame_mainmenu->render();
  frame_newgame = std::shared_ptr<NewGameMenu>(new NewGameMenu);
  frame_game = std::shared_ptr<GameFrame>(new GameFrame);
  frame_gen = std::shared_ptr<GenFrame>(new GenFrame);
  frame_load = std::shared_ptr<LoadFrame>(new LoadFrame);
  current_frame = frame_mainmenu;
  running = true;
  while(!context->getEventClose() && running)
    {
      context->pollEvents();
      context->use();
      current_frame->update(context);
      current_frame->render();
      context->flip();
    }
  frame_mainmenu.reset();
  frame_newgame.reset();
  frame_game.reset();
  frame_gen.reset();
  game.reset();
  current_frame.reset();
  graphics::u_ortho.reset();
  delete context;
	
  return EXIT_SUCCESS;
}
