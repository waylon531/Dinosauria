#pragma once
/** @file graphics/material.hpp Material handler */

#include "graphics/opengl.hpp"
#include "graphics/glsl.hpp"
#include "graphics/texture.hpp"
#include "graphics/fbo.hpp"

namespace graphics
{

  /** A material */
  class Material
  {
  private:

    /** The shader */
    std::unique_ptr<GLSL> shader;

    /** The array of uniforms */
    std::vector<std::shared_ptr<GLSLUniform>> uniforms;

    /** The array of textures */
    std::vector<std::shared_ptr<GLTexture>> textures;

    /** The model matrix uniform */
    std::shared_ptr<GLSLUniform> u_model;
    /** The view matrix uniform */
    std::shared_ptr<GLSLUniform> u_view;
    /** The projection matrix uniform */
    std::shared_ptr<GLSLUniform> u_project;
    /** The eye uniform */
    std::shared_ptr<GLSLUniform> u_eye;
    /** The sun uniform */
    std::shared_ptr<GLSLUniform> u_sun;
    /** The shadow texture uniform */
    std::shared_ptr<GLSLUniform> u_shadow[N_SHADOW_BUFFERS];
    /** The light matrix uniform */
    std::shared_ptr<GLSLUniform> u_lightMatrix[N_SHADOW_BUFFERS];

  public:

    /** Initialize from a material description file
     * @param fname filename of material */
    Material(const std::string& fname);

    /** Destroy */
    ~Material();

    /** Set the model matrix
     * @param m matrix pointer */
    void setMatrixModel(glm::mat4* m);
    
    /** Set the view matrix
     * @param m matrix pointer */
    void setMatrixView(glm::mat4* m);
    
    /** Set the projection matrix
     * @param m matrix pointer */
    void setMatrixProject(glm::mat4* m);

    /** Set the eye direction
     * @param eye eye vector */
    void setEyeDir(glm::vec3* dir);

    /** Set the sun direction
     * @param sun sun direction */
    void setSunDir(glm::vec3* dir);

    /** Set the shadow texture id
     * @param tex texture id */
    void setShadowTex(GLint* tex);

    /** Set the light matrix
     * @param m matrix pointer */
    void setMatrixLight(glm::mat4* m);

    /** Add a texture to this material
     * @param tex texture to add */
    void addTexture(std::shared_ptr<GLTexture> tex);

    /** Add a uniform to this material
     * @param u uniform to add */
    void addUniform(std::shared_ptr<GLSLUniform> u);
    
    /** Set the material as active */
    void use();

  };
};
    
