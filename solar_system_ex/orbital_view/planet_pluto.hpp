#ifndef __PLANET_PLUTO_H__
#define __PLANET_PLUTO_H__

#include "planet_solar_system.hpp"

#define PLUTO_TEXTURE_FILENAME "../texture/pluto.jpg"

//===================================================================================
// CPlanetPluto
//===================================================================================

class CPlanetPluto
  : public CPlanetSolarSystem
{
public:
  CPlanetPluto() : CPlanetSolarSystem() {}
  ~CPlanetPluto(){};
  
  void Init(void);
  
  static const double RADIUS = 0.450000;
  
  static const int SLICES = 90;
  static const int STACKS = 45;
  
  static const double ROTATION_RATE = 56.362724;
  static const double ORBITAL_PERIOD = 90700.0;
  static const double ASCENDING_NODE = 110.383302;
  static const double ORBIT_TILT = 17.121370;
  static const double PERIHELION = 224.802494;
  static const double MEAN_DISTANCE = 39.580400;
  static const double ECCENTRICITY = 0.250127;
  
};

#endif
