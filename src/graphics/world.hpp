#pragma once
/** @file graphics/world.hpp */

#include "graphics/mesh.hpp"
#include "graphics/camera.hpp"
#include "graphics/fbo.hpp"

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

    /** The shadow buffer list */
    std::unique_ptr<GLFrameBuffer> b_shadow[N_SHADOW_BUFFERS];

    /** The list of depth texture ids */
    GLint depthTexArray[N_SHADOW_BUFFERS];

    /** The camera for this world */
    std::shared_ptr<Camera> camera;

    /** The sun for this world */
    glm::vec3 sun;

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
