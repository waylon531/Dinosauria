#pragma once
/** @file game/game.hpp */

#include "game/dinosaur.hpp"
#include "game/landscape.hpp"
#include "game/water.hpp"
#include "graphics/world.hpp"
#include "graphics/sky.hpp"
#include "graphics/fbo.hpp"
#include "graphics/post.hpp"

/** A strcture to hold all data needed for a specific game */
class Game
{
private:

  std::shared_ptr<DinosaurInstance> player;
  std::shared_ptr<graphics::Sky> sky;

  std::shared_ptr<graphics::World> world;

  std::shared_ptr<graphics::Camera> camera;

  std::shared_ptr<Landscape> ground;
  std::shared_ptr<Ocean> ocean;

  std::shared_ptr<graphics::GLFrameBuffer> fbo;
  std::shared_ptr<graphics::GLFrameBuffer> fbo_reflection;
  std::shared_ptr<graphics::GLFrameBuffer> fbo_Cwater;
  std::shared_ptr<graphics::GLPass> pass_color;
  std::shared_ptr<graphics::GLPass> pass_color_reflection;
  std::shared_ptr<graphics::GLPass> pass_Cwater_color;
  std::shared_ptr<graphics::GLPass> pass_Cwater_depth;
  std::shared_ptr<graphics::GLSL> shader_Cwater;
  std::shared_ptr<graphics::GLSL> shader_Cfog;
  std::shared_ptr<graphics::Compositor> comp_water;
  std::shared_ptr<graphics::Compositor> comp_fog;
  std::shared_ptr<graphics::Compositor> comp_bloom;
  std::shared_ptr<graphics::GLSL> shader_Cbloom_sub;
  std::shared_ptr<graphics::GLSL> shader_Cbloom_hblur;
  std::shared_ptr<graphics::GLSL> shader_Cbloom_vblur;
  std::shared_ptr<graphics::GLSL> shader_Cbloom_final;
  std::shared_ptr<graphics::GLPass> pass_Cbloom_color_sub;
  std::shared_ptr<graphics::GLPass> pass_Cbloom_color_hblur;
  std::shared_ptr<graphics::GLPass> pass_Cbloom_color_vblur;
  std::shared_ptr<graphics::GLPass> pass_Cbloom_color_final;
  std::shared_ptr<graphics::GLFrameBuffer> fbo_Cbloom_sub;
  std::shared_ptr<graphics::GLFrameBuffer> fbo_Cbloom_hblur;
  std::shared_ptr<graphics::GLFrameBuffer> fbo_Cbloom_vblur;
  std::shared_ptr<graphics::GLFrameBuffer> fbo_Cbloom_final;

  std::shared_ptr<graphics::GLSL> shader_Cdof_down;
  std::shared_ptr<graphics::GLSL> shader_Cdof_hblur;
  std::shared_ptr<graphics::GLSL> shader_Cdof_vblur;
  std::shared_ptr<graphics::GLSL> shader_Cdof_final;
  std::shared_ptr<graphics::GLPass> pass_Cdof_color_down;
  std::shared_ptr<graphics::GLPass> pass_Cdof_color_hblur;
  std::shared_ptr<graphics::GLPass> pass_Cdof_color_vblur;
  std::shared_ptr<graphics::GLFrameBuffer> fbo_Cdof_down;
  std::shared_ptr<graphics::GLFrameBuffer> fbo_Cdof_hblur;
  std::shared_ptr<graphics::GLFrameBuffer> fbo_Cdof_vblur;
  
  std::shared_ptr<graphics::Compositor> comp_basic;
  
  /** Get the ideal camera position
   * @return ideal location of a camera */
  glm::vec3 getCameraLoc();
  /** The camera's focal length */
  float focalLength;
  /** The camera's distance from the player */
  float camScale;

  int time;

public:

  /** Initialize a blank new game */
  Game();

  ~Game();

  /** Start a new game */
  void start(float* progValue, float* value2, Callback& callback);

  /** Initialize OpenGL components */
  void initGL();

  /** Save the game to a directory
   * @param name name of the directory */
  void save(const std::string& name);

  /** Load the game from a saved directory
   * @param dir directory */
  void load(const std::string& dir, float* progValue, float* value2, Callback& callback);

  /** Set the game's dinosaur
   * @param dino pointer to the base class for the player's dinosaur
   * @param this will make a copy of it */
  void setPlayer(std::shared_ptr<Dinosaur> dino);

  /** Render the game world */
  void render();

  /** Reset the whole game */
  void resetGame();

  void onMouseClick(const glm::vec2& pos);
  void onMouseRelease(const glm::vec2& pos);
  void onMouseOver(const glm::vec2& pos);
  void onNotMouseOver();
  void onKey(GLFWwindow* window);
};
