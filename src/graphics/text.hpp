#pragma once
/** @brief graphics/text.hpp freetype-gl wrapper for text rendering */

#include "freetype-gl/freetype-gl.h"

#include "util.hpp"
#include "graphics/opengl.hpp"
#include "graphics/types.hpp"
#include "graphics/rendObj.hpp"
#include "graphics/buffer.hpp"
#include "graphics/ibo.hpp"
#include "graphics/glsl.hpp"

namespace graphics
{

  void initAtlas();

  /** @brief A font wrapper */
  class DynamicFont
  {
  public:
    
    /** The font's texture atlas */
    texture_atlas_t* atlas;

    /** The font itself */
    texture_font_t* font;

    /** Construct from a true type font filename
     * @param fname filename of a ttf
     * @param size font face size */
    DynamicFont(const std::string& fname, const int size=20);

    /** Destroy */
    ~DynamicFont();
  };

  /** @brief Static text storage
   * The text itself must remain the same but the position and color can change*/
  class StaticText: public RenderableObject
  {
  private:

    /** The DynamicFont object to use */
    std::shared_ptr<DynamicFont> font;

    /** The vertex buffer object */
    std::unique_ptr<GLVertexBuffer> vBuffer;
    /** The index buffer object */
    std::unique_ptr<GLIndexBuffer> iBuffer;
    /** The GLSL shader object */
    std::unique_ptr<GLSL> shader;

  public:

    /** The position to render to */
    glm::vec2 pos;

    /** The text color */
    Color color;

    /** The bounding box of the text */
    BBox2D bbox;

    /** Construct from a font only
     * @param f font to use
     * @param text text to draw */
    StaticText(std::shared_ptr<DynamicFont> f, const std::string& text, glm::vec2 Pos, const Color& Ccolor, std::shared_ptr<GLSLUniform> uniform=u_ortho);

    /** Destroy */
    ~StaticText();

    void render();
  };

  /** Text with changable content */
  class DynamicText: public RenderableObject
  {
  private:

    /** The DynamicFont object to use */
    std::shared_ptr<DynamicFont> font;

    /** The vertex buffer object */
    std::unique_ptr<GLVertexBufferDynamic> vBuffer;
    /** The index buffer object */
    std::unique_ptr<GLIndexBufferDynamic> iBuffer;
    /** The GLSL shader object */
    std::unique_ptr<GLSL> shader;

  public:

    /** The bounding box of the text */
    BBox2D bbox;

    /** Construct from a font only
     * @param f font to use
     * @param text text to draw */
    DynamicText(std::shared_ptr<DynamicFont> f, std::shared_ptr<GLSLUniform> uniform=u_ortho);

    /** Destroy */
    ~DynamicText();

    /** Change the text contents */
    void setText(const std::string& text, glm::vec2 pos, const Color& color);

    void render();
  };
};
