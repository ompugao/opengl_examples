#ifndef __PLANET_NEPTUNE_H__
#define __PLANET_NEPTUNE_H__

#include "planet_solar_system.hpp"

#define NEPTUNE_TEXTURE_FILENAME "../texture/neptune.jpg"

//===================================================================================
// CPlanetNeptune
//===================================================================================

class CPlanetNeptune
  : public CPlanetSolarSystem
{
public:
  CPlanetNeptune() : CPlanetSolarSystem() {}
  ~CPlanetNeptune(){};
  
  void Init(void);
  
  static const double RADIUS = 4.000000;
  
  static const int SLICES = 90;
  static const int STACKS = 45;
  
  static const double ROTATION_RATE = 536.312866;
  static const double ORBITAL_PERIOD = 60188.000000;
  static const double ASCENDING_NODE = 131.792503;
  static const double ORBIT_TILT = 1.768100;
  static const double PERIHELION = 7.206000;
  static const double MEAN_DISTANCE = 30.266640;
  static const double ECCENTRICITY = 0.010298;
  
};

#endif
