#include <stdio.h>
#include <math.h>

#include "sun.hpp"

//-----------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------
void CSun::Init(void)
{
  m_radius = RADIUS;
  
  m_slices = SLICES;
  m_stacks = STACKS;
  
  m_rotation_rate = ROTATION_RATE;
  
  LoadPlanetTexture( SUN_TEXTURE_FILENAME );
  
  m_b_texture = true;
  m_b_wire = false;
}

//-----------------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------------
void CSun::Draw(void)
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
}
