#include "planet_neptune.hpp"

//-----------------------------------------------------------------------------------
// �����
//-----------------------------------------------------------------------------------
void CPlanetNeptune::Init(void)
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
  
  LoadPlanetTexture( NEPTUNE_TEXTURE_FILENAME );
}
