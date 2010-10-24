#ifndef __PLANET_JUPITER_H__
#define __PLANET_JUPITER_H__

#include "planet_solar_system.hpp"

#define JUPITER_TEXTURE_FILENAME "../texture/jupiter.jpg"

//===================================================================================
// CPlanetJupiter
//===================================================================================

class CPlanetJupiter
  : public CPlanetSolarSystem
{
public:
  CPlanetJupiter() : CPlanetSolarSystem() {}
  ~CPlanetJupiter(){};
  
  void Init(void);
  
  static const double RADIUS = 11.27000;
  
  static const int SLICES = 90;
  static const int STACKS = 45;
  
  static const double ROTATION_RATE = 870.532471;
  static const double ORBITAL_PERIOD = 4332.586914;
  
  static const double ASCENDING_NODE = 100.471297;
  static const double ORBIT_TILT = 1.304630;
  static const double PERIHELION = 15.697800;
  static const double MEAN_DISTANCE = 5.202597;
  static const double ECCENTRICITY = 0.048465;
  
};

#endif
