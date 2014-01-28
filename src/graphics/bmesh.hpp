#pragma once
/** @file graphics/bmesh.hpp Triangle mesh with skeletal animation support */

#include "graphics/opengl.hpp"
#include "graphics/glsl.hpp"
#include "graphics/buffer.hpp"
#include "graphics/ibo.hpp"
#include "graphics/texture.hpp"
#include "graphics/rendObj.hpp"
#include "graphics/material.hpp"

namespace graphics
{

  /** A triangle based mesh
   * This will handle all rendering so you only need to call render() */
  class SkeletalMesh: public RenderableObjectExt
  {
  public:
    
    struct attrib
    {
      glm::vec3 pos;
      glm::vec3 normal;
      glm::vec2 texCoord;
      int boneId[4];
      float boneWeight[4];
    };
    struct Bone
    {
      glm::mat4 offset;
      Bone(glm::mat4 o) : offset(o)
      {
      }
    };

    struct RotationKeyframe
    {
      glm::quat value;
      float time;
      RotationKeyframe()
      {
      }
    };
    
    struct TranslationKeyframe
    {
      glm::vec3 value;
      float time;
      TranslationKeyframe()
      {
      }
    };
    
    
    struct ActionNode
    {
      int boneId;
      std::vector<RotationKeyframe> rotkeys;
      std::vector<TranslationKeyframe> lockeys;
      std::vector<int> children;
      ActionNode()
      {
      }
      ActionNode(int id, std::vector<int> c, std::vector<RotationKeyframe> rkeys, std::vector<TranslationKeyframe> lkeys) : boneId(id), children(c), rotkeys(rkeys), lockeys(lkeys)
      {
      }
    };
    struct Action
    {
      std::vector<ActionNode> nodes;
      Action()
      {
      }
    };
    
  private:

    /** The vertex array */
    attrib* verts;

    /** The index array */
    GLuint* inds;

    /** The bone array */
    std::vector<Bone> bones;
    Action anim;

    /** The vertex buffer */
    std::unique_ptr<GLVertexBuffer> b_vert;

    /** The index buffer */
    std::unique_ptr<GLIndexBuffer> b_ind;

    /** The shadow shader */
    std::unique_ptr<GLSL> shader_shadow;
    /** The light matrix uniform */
    std::shared_ptr<GLSLUniform> u_light;

    /** Initialize the shadow shader */
    void initShadow();

    /** Get the interpolated rotation matrix
     * @param time the current time
     * @param node the action node to interpolate on
     * @return rotation quaternion */
    glm::quat interpRotation(const float time, const ActionNode& node) const;

    /** Get the interpolated translate matrix
     * @param time the current time
     * @param node the action node to interpolate on
     * @return translation vector */
    glm::vec3 interpTranslation(const float time, const ActionNode& node) const;

    /** Get the full interpolated matrix
     * @param time the current time
     * @param node the action node to interpolate on
     * @return interpolated animation matrix for the bone */
    glm::mat4 interpMatrix(const float time, const ActionNode& node) const;

    /** Read the heirarchy of nodes and set their transforms
     * @param node the node to be reading from
     * @param ptrans the parent's transformation matrix
     * @param time animation time */
    void readNodes(const ActionNode& node, const glm::mat4& ptrans, const float time);
    
  public:

    /** The model matrix */
    glm::mat4 m_model;

    /** Initialize from a file
     * @param fname filename for mesh
     * @param mat material for this mesh */
    SkeletalMesh(const std::string& fname, std::shared_ptr<Material> mat, bool isBin=false);

    /** Initialize from veritces and indices
     * @param v vertex array
     * @param i index array
     * @param nv number of vertices
     * @param ni number of indices */
    //SkeletalMesh(attrib* v, GLuint* i, const int nv, const int ni, std::shared_ptr<Material>);

    /** Destroy */
    ~SkeletalMesh();

    /** Set the animation time */
    void setTime(const float t);

    void render();
    void renderShadowPass();
    void setMatrixView(glm::mat4* view);
    void setMatrixProject(glm::mat4* project);
    void setWaterFlag(int* flag);
    
    /** The material of the mesh */
    std::shared_ptr<Material> material;
    
  };
};
