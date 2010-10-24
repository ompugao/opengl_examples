#include "planet_custom.hpp"

//-----------------------------------------------------------------------------------
// �ǥե���ȥ��󥹥ȥ饯��
//-----------------------------------------------------------------------------------
CPlanetCustom::CPlanetCustom()
  :CPlanet()
{
  m_b_wire = false;
}

//-----------------------------------------------------------------------------------
// �����
//-----------------------------------------------------------------------------------
void CPlanetCustom::Init(void)
{
  m_radius = RADIUS;
  
  m_slices = SLICES;
  m_stacks = STACKS;
  
  LoadPlanetTexture( "../texture/earthclouds_1024.jpg" );
}

//-----------------------------------------------------------------------------------
// ����
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
