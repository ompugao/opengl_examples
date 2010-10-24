#include "planet_mars.hpp"

//-----------------------------------------------------------------------------------
// ½é´ü²½
//-----------------------------------------------------------------------------------
void CPlanetMars::Init(void)
{
  m_radius = RADIUS;
  
  m_slices = SLICES;
  m_stacks = STACKS;
  
  m_rotation_rate = ROTATION_RATE;
  m_orbital_period = ORBITAL_PERIOD;
  m_ascending_node = ASCENDING_NODE;
  m_orbit_tilt = ORBIT_TILT;
  m_perihelion = PERIHELION;
  m_mean_distance = MEAN_DISTANCE;
  m_eccentricity = ECCENTRICITY;
  
  LoadPlanetTexture( MARS_TEXTURE_FILENAME );
}
