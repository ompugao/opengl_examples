#ifndef __PLANET_MARS_H__
#define __PLANET_MARS_H__

#include "planet_solar_system.hpp"

#define MARS_TEXTURE_FILENAME "../texture/mars.jpg"

//===================================================================================
// CPlanetMars
//===================================================================================

class CPlanetMars
  : public CPlanetSolarSystem
{
public:
  CPlanetMars() : CPlanetSolarSystem() {}
  ~CPlanetMars(){};
  
  void Init(void);
  
  static const double RADIUS = 0.532000;
  
  static const int SLICES = 90;
  static const int STACKS = 45;
  
  static const double ROTATION_RATE = 350.891907;
  static const double ORBITAL_PERIOD = 686.979980;
  static const double ASCENDING_NODE = 49.566399;
  static const double ORBIT_TILT = 1.849920;
  static const double PERIHELION = 336.088201;
  static const double MEAN_DISTANCE = 1.523636;
  static const double ECCENTRICITY = 0.093423;
  
};

#endif
