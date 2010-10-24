#include "planet_custom.hpp"

//-----------------------------------------------------------------------------------
// デフォルトコンストラクタ
//-----------------------------------------------------------------------------------
CPlanetCustom::CPlanetCustom()
  :CPlanet()
{
  m_b_wire = false;
}

//-----------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------
void CPlanetCustom::Init(void)
{
  m_radius = RADIUS;
  
  m_slices = SLICES;
  m_stacks = STACKS;
  
  LoadPlanetTexture( "../texture/earthclouds_1024.jpg" );
}

//-----------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------
void CPlanetCustom::Draw(void)
{
  if( m_b_wire == false ) {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glEnable(GL_TEXTURE_2D);
    
    glEnable( GL_LIGHTING);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  }
  else {
    glDisable(GL_TEXTURE_2D);
    glColor3f( 1.0, 1.0, 1.0 );
    
    glEnable( GL_LIGHTING);
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  }
  
  DrawPlanetSphere();
}
