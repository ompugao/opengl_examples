#ifndef __PLANET_VENUS_H__
#define __PLANET_VENUS_H__

#include "planet_solar_system.hpp"

#define VENUS_TEXTURE_FILENAME "../texture/venus.jpg"

//===================================================================================
// CPlanetVenus
//===================================================================================

class CPlanetVenus
  : public CPlanetSolarSystem
{
public:
  CPlanetVenus() : CPlanetSolarSystem() {}
  ~CPlanetVenus(){};
  
  void Init(void);
  
  static const double RADIUS = 0.950000;
  
  static const int SLICES = 90;
  static const int STACKS = 45;
  
  static const double ROTATION_RATE = 1.481367;
  static const double ORBITAL_PERIOD = 224.699997;
  static const double ASCENDING_NODE = 76.688900;
  static const double ORBIT_TILT = 3.394720;
  static const double PERIHELION = 131.761002;
  static const double MEAN_DISTANCE = 0.723324;
  static const double ECCENTRICITY = 0.006793;
  
};

#endif
