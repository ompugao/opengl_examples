#ifndef __PLANET_SOLAR_SYSTEM_H__
#define __PLANET_SOLAR_SYSTEM_H__

#include "planet.hpp"

//===================================================================================
// CPlanetSolarSystem
//===================================================================================

class CPlanetSolarSystem
  : public CPlanet
{
public:
  CPlanetSolarSystem();
  virtual ~CPlanetSolarSystem(){};
  
  void Draw(void);
  
  void setRenderTexture( bool val ) {m_b_texture = val; }
  void setRenderWire( bool val ) {m_b_wire = val; }
  void setRenderOrbit( bool val ) {m_b_orbit = val; }

private:
  
  //
  bool m_b_texture;
  bool m_b_wire;
  bool m_b_orbit;
  
};

#endif
