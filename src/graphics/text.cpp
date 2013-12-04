#include "graphics/text.hpp"

texture_atlas_t* gAtlas;
void graphics::initAtlas()
{
  const int w = pow(2,11);
  gAtlas = texture_atlas_new(w,w,1);
}

graphics::DynamicFont::DynamicFont(const std::string& fname, const int size)
{
  //float w = powf(2,log2f(size*8));
  //atlas = texture_atlas_new(w,w,4);
  atlas = gAtlas;
  font = texture_font_new(atlas, fname.c_str(), size);
  //load the needed glyphs
  wchar_t glyphs[93];
  for(int g=0; g<93; g++)
    {
      glyphs[g] = wchar_t(g+33);
    }
  texture_font_load_glyphs(font, glyphs);
}

graphics::DynamicFont::~DynamicFont()
{
  texture_font_delete(font);
}

graphics::StaticText::StaticText(std::shared_ptr<DynamicFont> f, const std::string& text, glm::vec2 Pos, const Color& Ccolor, std::shared_ptr<GLSLUniform> uniform)
{
  font = f;
  wchar_t* textT = getWchar(text);
  pos = Pos;
  color = Ccolor;

  //initialize shader
  shader = std::unique_ptr<GLSL>(new GLSL(0));
  shader->addShader("res/shaders/text.vert",GL_VERTEX_SHADER);
  shader->addShader("res/shaders/text.frag",GL_FRAGMENT_SHADER);
  shader->linkShader();
  std::shared_ptr<GLSLUniform> texId(new GLSLUniform("tex0",(GLint*)&font->font->atlas->id));
  shader->attachUniform(texId);
  shader->attachUniform(uniform);
  
  //figure out how many vertices we will have
  int nv = 4 * text.size();
  int nt = 6 * text.size();
  struct attrib
  {
    glm::vec2 pos;
    glm::vec2 tex_coord;
    Color color;
  };
  GLSLAttributeSet attrs(std::vector<GLSLAttribute>({GLSLAttribute("pos",2), GLSLAttribute("tex_coord",2), GLSLAttribute("color",4)}));
  attrib* data = new attrib[nv];
  unsigned int* inds = new unsigned int[nt];
  //fill data
  glm::vec2 oldPos = pos;
  float width = 0.0f;
  float maxHeight = 0.0f;
  const float& r=color.r; const float& g=color.g; const float& b=color.b; const float& a=color.a;
  for(int i=0; i<text.size(); i++)
    {
      texture_glyph_t* glyph = texture_font_get_glyph(font->font, textT[i]);
      if(glyph != NULL)
	{
	  int kerning = 0;
	  if(i > 0) kerning = texture_glyph_get_kerning(glyph, textT[i-1]);
	  pos.x += kerning;
	  float x0  = (pos.x + glyph->offset_x );
	  float y0  = (pos.y + glyph->offset_y );
	  float x1  = ( x0 + glyph->width );
	  float y1  = ( y0 - glyph->height );
	  float s0 = glyph->s0;
	  float t0 = glyph->t0;
	  float s1 = glyph->s1;
	  float t1 = glyph->t1;

	  inds[i*6+0]=0+i*4;
	  inds[i*6+1]=1+i*4;
	  inds[i*6+2]=2+i*4;
	  inds[i*6+3]=0+i*4;
	  inds[i*6+4]=2+i*4;
	  inds[i*6+5]=3+i*4;
	  
	  data[i*4+0] = (attrib){glm::vec2(x0,y0),  glm::vec2(s0,t0), Color(r,g,b,a)};
	  data[i*4+1] = (attrib){glm::vec2(x0,y1),  glm::vec2(s0,t1), Color(r,g,b,a)};
	  data[i*4+2] = (attrib){glm::vec2(x1,y1),  glm::vec2(s1,t1), Color(r,g,b,a)};
	  data[i*4+3] = (attrib){glm::vec2(x1,y0),  glm::vec2(s1,t0), Color(r,g,b,a)};
	  pos.x += glyph->advance_x;
	  width += kerning + glyph->advance_x;
	  maxHeight = max(maxHeight, (float)glyph->height);
	}
    }
  bbox = BBox2D(oldPos, width,maxHeight);
  //initialize vertex buffer
  vBuffer = std::unique_ptr<GLVertexBuffer>(new GLVertexBuffer((void*)data, nv, attrs));
  iBuffer = std::unique_ptr<GLIndexBuffer>(new GLIndexBuffer(inds, nt));
}

graphics::StaticText::~StaticText()
{
  font.reset();
  vBuffer.reset();
  iBuffer.reset();
  shader.reset();
}

void graphics::StaticText::render()
{
  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0+font->atlas->id);
  glDisable(GL_DEPTH_TEST);
  glBindTexture(GL_TEXTURE_2D, font->atlas->id);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  shader->use();
  vBuffer->use();
  iBuffer->use();
  iBuffer->draw(GL_TRIANGLES);
  vBuffer->unuse();
  iBuffer->unuse();
}

graphics::DynamicText::DynamicText(std::shared_ptr<DynamicFont> f, std::shared_ptr<GLSLUniform> uniform)
{
  font = f;
  GLSLAttributeSet attrs(std::vector<GLSLAttribute>({GLSLAttribute("pos",2), GLSLAttribute("tex_coord",2), GLSLAttribute("color",4)}));
  vBuffer = std::unique_ptr<GLVertexBufferDynamic>(new GLVertexBufferDynamic(attrs));
  iBuffer = std::unique_ptr<GLIndexBufferDynamic>(new GLIndexBufferDynamic);
  //initialize shader
  shader = std::unique_ptr<GLSL>(new GLSL(0));
  shader->addShader("res/shaders/text.vert",GL_VERTEX_SHADER);
  shader->addShader("res/shaders/text.frag",GL_FRAGMENT_SHADER);
  shader->linkShader();
  std::shared_ptr<GLSLUniform> texId(new GLSLUniform("tex0",&font->font->atlas->id));
  shader->attachUniform(texId);
  shader->attachUniform(uniform);
}

graphics::DynamicText::~DynamicText()
{
  font.reset();
  vBuffer.reset();
  iBuffer.reset();
  shader.reset();
}

void graphics::DynamicText::setText(const std::string& text, glm::vec2 pos, const Color& color)
{
  wchar_t* textT = getWchar(text);

  
  //figure out how many vertices we will have
  int nv = 4 * text.size();
  int nt = 6 * text.size();
  struct attrib
  {
    Point2D pos;
    Point2D tex_coord;
    Color color;
  };
  attrib* data = new attrib[nv];
  unsigned int* inds = new unsigned int[nt];
  //fill data
  glm::vec2 oldPos = pos;
  float width = 0.0f;
  float maxHeight = 0.0f;
  const float& r=color.r; const float& g=color.g; const float& b=color.b; const float& a=color.a;
  for(int i=0; i<text.size(); i++)
    {
      texture_glyph_t* glyph = texture_font_get_glyph(font->font, textT[i]);
      if(glyph != NULL)
	{
	  int kerning = 0;
	  if(i > 0) kerning = texture_glyph_get_kerning(glyph, textT[i-1]);
	  pos.x += kerning;
	  float x0  = (pos.x + glyph->offset_x );
	  float y0  = (pos.y + glyph->offset_y );
	  float x1  = ( x0 + glyph->width );
	  float y1  = ( y0 - glyph->height );
	  float s0 = glyph->s0;
	  float t0 = glyph->t0;
	  float s1 = glyph->s1;
	  float t1 = glyph->t1;

	  inds[i*6+0]=0+i*4;
	  inds[i*6+1]=1+i*4;
	  inds[i*6+2]=2+i*4;
	  inds[i*6+3]=0+i*4;
	  inds[i*6+4]=2+i*4;
	  inds[i*6+5]=3+i*4;
	  
	  data[i*4+0] = (attrib){Point2D(x0,y0),  Point2D(s0,t0), Color(r,g,b,a)};
	  data[i*4+1] = (attrib){Point2D(x0,y1),  Point2D(s0,t1), Color(r,g,b,a)};
	  data[i*4+2] = (attrib){Point2D(x1,y1),  Point2D(s1,t1), Color(r,g,b,a)};
	  data[i*4+3] = (attrib){Point2D(x1,y0),  Point2D(s1,t0), Color(r,g,b,a)};
	  pos.x += glyph->advance_x;
	  width += kerning + glyph->advance_x;
	  maxHeight = max(maxHeight, (float)glyph->height);
	}
    }
  bbox = BBox2D(oldPos, width,maxHeight);
  //initialize vertex buffer
  vBuffer->update((void*)data, nv);
  iBuffer->update(inds, nt);
}

void graphics::DynamicText::render()
{
  glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0+font->atlas->id);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  shader->use();
  vBuffer->use();
  iBuffer->use();
  iBuffer->draw(GL_TRIANGLES);
  vBuffer->unuse();
  iBuffer->unuse();
}
