#ifndef __PLANET_MERCURY_H__
#define __PLANET_MERCURY_H__

#include "planet_solar_system.hpp"

#define MERCURY_TEXTURE_FILENAME "../texture/mercury.jpg"

//===================================================================================
// CPlanetMercury
//===================================================================================

class CPlanetMercury
  : public CPlanetSolarSystem
{
public:
  CPlanetMercury() : CPlanetSolarSystem() {}
  ~CPlanetMercury(){};
  
  void Init(void);
  
  static const double RADIUS = 0.382000;
  
  static const int SLICES = 90;
  static const int STACKS = 45;

  static const double ROTATION_RATE = 6.138505;
  static const double ORBITAL_PERIOD = 87.969002;
  static const double ASCENDING_NODE = 48.333899;
  static const double ORBIT_TILT = 0.0;
  static const double PERIHELION = 77.453997;
  static const double MEAN_DISTANCE = 0.387098;
  static const double ECCENTRICITY = 0.205632;
  
};

#endif
