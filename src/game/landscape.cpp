#include "game/landscape.hpp"

Landscape::Landscape() : isWireframe(0)
{
  material = std::unique_ptr<graphics::Material>(new graphics::Material("res/materials/ground.mat"));
  material_shadow = std::unique_ptr<graphics::Material>(new graphics::Material("res/materials/ground_shadow.mat"));
  material->addUniform(std::shared_ptr<graphics::GLSLUniform>(new graphics::GLSLUniform("tessMax", &tessMax)));
  material->addUniform(std::shared_ptr<graphics::GLSLUniform>(new graphics::GLSLUniform("isWireframe",&isWireframe)));
  m_tessView = new glm::mat4;
  material_shadow->addUniform(MKPTR(graphics::GLSLUniform, "m_tessView", m_tessView));
  //material->addTexture(std::shared_ptr<graphics::GLTexture2D>(new graphics::GLTexture2D("res/textures/dirt.png","tex_dirt")));

#define SCALE 500.f
#define HSCALE (.05f*SCALE)
#define RES (1024*2)
  //#define RES 128
#define INIT_RES 128
#define RES_RATIO (RES/INIT_RES)
#define PERSIST 0.4f
  verts = new attrib[INIT_RES*INIT_RES];
  inds = new GLuint[INIT_RES*INIT_RES*4];
  nmap = new glm::vec3[RES*RES];
  tmap = new glm::vec3[RES*RES];
  btmap = new glm::vec3[RES*RES];
  
  hasLoaded = false;
}

#define INIT_HEIGHT_TEX tex_height = std::shared_ptr<graphics::GLTexture2D>(new graphics::GLTexture2D(hmap, RES,RES, "tex_height", GL_RGBA32F ,GL_RED, GL_FLOAT));
#define INIT_MIX_TEX tex_b = std::shared_ptr<graphics::GLTexture2D>(new graphics::GLTexture2D(bmap, RES,RES, "tex_blend", GL_RGBA ,GL_RED, GL_FLOAT));

void Landscape::initMesh(float* value, Callback& callback)
{
  computeGradient(hmap,RES,RES,tmap,btmap,nmap, value,callback);
  INIT_HEIGHT_TEX;
  material_shadow->addTexture(tex_height);
  material->addTexture(tex_height);
  INIT_MIX_TEX;
  material->addTexture(tex_b);
  tex_normal = std::shared_ptr<graphics::GLTexture2D>(new graphics::GLTexture2D(nmap, RES,RES, "tex_normal", GL_RGBA,GL_RGB));
  material->addTexture(tex_normal);
  material_shadow->addTexture(tex_normal);
  for(int xx=0; xx<INIT_RES; xx++)
    {
      int x = RES_RATIO * xx;
      for(int yy=0; yy<INIT_RES; yy++)
	{
	  int y = RES_RATIO * yy;
	  glm::vec3 vpos = glm::vec3(((float)x-RES/2.f)*(SCALE/RES), 0.0f, ((float)y-RES/2.f)*(SCALE/RES));
	  verts[xx*INIT_RES+yy] = (attrib){vpos, glm::vec2(y/(float)RES, x/(float)RES), nmap[x*RES+y]};
	  inds[(xx*INIT_RES+yy)*4+0] = xx*INIT_RES+yy;
	  inds[(xx*INIT_RES+yy)*4+1] = clamp(xx+1,0,INIT_RES-1)*INIT_RES+yy;
	  inds[(xx*INIT_RES+yy)*4+2] = xx*INIT_RES+clamp(yy+1,0,INIT_RES-1);
	  inds[(xx*INIT_RES+yy)*4+3] = clamp(xx+1,0,INIT_RES-1)*INIT_RES+clamp(yy+1,0,INIT_RES-1);
	  /*inds[(xx*INIT_RES+yy)*6+4] = clamp(xx+1,0,INIT_RES-1)*INIT_RES+yy;
	    inds[(xx*INIT_RES+yy)*6+5] = xx*INIT_RES+clamp(yy+1,0,INIT_RES-1);*/
	}
    }  graphics::GLSLAttributeSet attrs(std::vector<graphics::GLSLAttribute>({graphics::GLSLAttribute("position",3), graphics::GLSLAttribute("texCoord",2), graphics::GLSLAttribute("normal",3)}));
  b_verts = std::unique_ptr<graphics::GLVertexBuffer>(new graphics::GLVertexBuffer(verts, INIT_RES*INIT_RES, attrs));
  b_inds = std::unique_ptr<graphics::GLIndexBuffer>(new graphics::GLIndexBuffer(inds, INIT_RES*INIT_RES*4));
  m_model = glm::mat4();
}  

void Landscape::generate(float* value, Callback& callback)
{
  hasLoaded = true;
  hmap = new float[RES*RES];
  bmap = new float[RES*RES];
  pgen::generateTerrain(RES,hmap,bmap,value,callback);
  for(int x=0; x<RES*RES; x++)
    {
      hmap[x] *= HSCALE;
      //hmap[x] = fabsf(hmap[x]);
    }
  initMesh(value,callback);
}

void Landscape::loadMap(const std::string& dir, float* value, Callback& callback)
{
  hasLoaded = true;
  hmap = readFloatFile(RES,RES,dir+"ground.float");
  bmap = readFloatFile(RES,RES,dir+"ground_b.float");
  initMesh(value,callback);
}


void Landscape::reset()
{
  if(hasLoaded)
    {
      tex_height.reset();
      tex_normal.reset();
      tex_b.reset();
      b_verts.reset();
      b_inds.reset();
      hasLoaded = false;
    }
}

Landscape::~Landscape()
{
  material.reset();
  material_shadow.reset();
  delete[] verts;
  delete[] inds;
  delete[] nmap;
  delete[] bmap;
  if(hasLoaded)
    {
      delete[] hmap;
      tex_height.reset();
      tex_normal.reset();
      b_verts.reset();
      b_inds.reset();
    }
}

void Landscape::renderShadowPass()
{
  tessMax = 1;
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  material_shadow->setMatrixModel(&m_model);
  material_shadow->use();
  b_verts->use();
  b_inds->use();
  glPatchParameteri(GL_PATCH_VERTICES, 4);
  b_inds->draw(GL_PATCHES);
  b_inds->unuse();
  b_verts->unuse();
  glDisable(GL_CULL_FACE);
}  

void Landscape::render()
{
  tessMax = 64;
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  //glCullFace(GL_BACK);
  material->setMatrixModel(&m_model);
  material->use();
  b_verts->use();
  b_inds->use();
  glPatchParameteri(GL_PATCH_VERTICES, 4);
  b_inds->draw(GL_PATCHES);
  b_inds->unuse();
  b_verts->unuse();
  glDisable(GL_CULL_FACE);
}

float Landscape::eval(const float x, const float y) const
{
  return lerpArray2D(hmap,RES,RES, (x/(SCALE/RES))+RES/2.f, (y/(SCALE/RES))+RES/2.f);
}

void Landscape::save(const std::string& dir)
{
  writeFloatFile(RES,RES,dir+"ground.float",hmap);
  writeFloatFile(RES,RES,dir+"ground_b.float",bmap);
}
