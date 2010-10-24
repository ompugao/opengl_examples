#include "planet_solar_system.hpp"

//-----------------------------------------------------------------------------------
// デフォルトコンストラクタ
//-----------------------------------------------------------------------------------
CPlanetSolarSystem::CPlanetSolarSystem()
  :CPlanet()
{
  m_b_texture = true;
  m_b_wire = false;
  m_b_orbit = true;
}

//-----------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------
void CPlanetSolarSystem::Draw(void)
{
  glEnable( GL_LIGHTING);
  
  if( m_b_texture ) {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE);
  }
  else {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE);
    glColor3f( 0.8, 0.8, 0.8 );
  }
  
  if( m_b_wire == false ) {
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  }
  else {
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  }
    
  DrawPlanetSphere();
  
  if( m_b_orbit )
    DrawOrbit();
}
