#pragma once
/** @file graphics/world.hpp */

#include "graphics/mesh.hpp"
#include "graphics/camera.hpp"
#include "graphics/fbo.hpp"
#include "graphics/post.hpp"

namespace graphics
{

  /** A class for 3D worlds
   * These are specifically designed to handle rendering actual 3D content, not GUIs or anything like that */
  class World: public RenderableObject
  {
  private:

    /** The list of meshes in this world */
    std::vector<std::shared_ptr<RenderableObjectExt>> meshs;

    /** The list of lights in this world */
    //std::vector<Light> lights;

    /** If there are shadows */
    bool hasShadows;

    /** A cached set of light matrices */
    glm::mat4 m_light2[N_SHADOW_BUFFERS];

  public:

    /** The rendering mode used */
    int renderMode;

    std::shared_ptr<graphics::GLFrameBuffer> fbo;
    std::shared_ptr<graphics::GLPass> pass_color;
    std::shared_ptr<graphics::GLPass> pass_normal;
    std::shared_ptr<graphics::GLPass> pass_params; 
    std::shared_ptr<graphics::GLPass> pass_position;
    std::shared_ptr<graphics::GLSL> shader_deffered;
    std::shared_ptr<graphics::Compositor> comp_deffered;

    /** The shadow buffer list */
    std::unique_ptr<GLFrameBuffer> b_shadow[N_SHADOW_BUFFERS];

    /** The list of depth texture ids */
    GLint depthTexArray[N_SHADOW_BUFFERS];

    /** The camera for this world */
    std::shared_ptr<Camera> camera;

    /** The sun for this world */
    glm::vec3 sun;

    /** If it is currently being rendered as a water pass */
    int isWaterPass;

    /** Initialize from settings
     * @param cam camera for this world
     * @param s whether shadows should be used */
    World(std::shared_ptr<Camera> cam, glm::vec3 sunDir=glm::vec3(0.f,1.f,0.f), const bool s=false);

    ~World();

    /** Add a mesh to the world
     * @param mesh mesh to add */
    void addMesh(std::shared_ptr<RenderableObjectExt> mesh);

    /** Remove a mesh from the world
     * @param mesh mesh to remove */
    void removeMesh(std::shared_ptr<RenderableObjectExt> mesh);

    /** Set up a mesh, but do not render it */
    void use(std::shared_ptr<RenderableObjectExt> mesh);
    
    void render();
  };
  
};
