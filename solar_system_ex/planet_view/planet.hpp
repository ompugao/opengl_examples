#ifndef __PLANER_H__
#define __PLANER_H__

#include <GL/glut.h>

//===================================================================================
// CPlanet
//===================================================================================

class CPlanet
{
public:
  
  CPlanet();
  virtual ~CPlanet();
  
  virtual void Init(void) = 0;
  
  virtual void Draw(void) = 0;
  
  // 代入&参照
  const double getRedius() const { return m_radius; }
  const double getSlices() const { return m_slices; }
  const double getStacks() const { return m_stacks; }
  
  const double getDay() const { return m_day; }
  
  void setRadius( double radius ) { m_radius = radius; }
  void setSlices( int slices ) { m_slices = slices; }
  void setStacks( int stacks ) { m_stacks = stacks; }
  
  void setDay( double day ) { m_day = day; };
  
protected:
  void LoadPlanetTexture( const char* filename );
  void DrawPlanetSphere();
  void DrawOrbit();
  
  // 球の半径
  double m_radius;
  
  // 球の分割
  int m_slices, m_stacks;
  
  // パラメータ
  double m_orbital_period;
  double m_rotation_rate;
  
  double m_ascending_node;
  double m_orbit_tilt;
  double m_perihelion;
  double m_mean_distance;
  double m_eccentricity;
  
  // 日
  double m_day;
  
  // テクスチャID
  GLuint m_texture;

};

#endif
