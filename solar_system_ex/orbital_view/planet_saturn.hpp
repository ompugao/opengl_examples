#ifndef __PLANET_SATURN_H__
#define __PLANET_SATURN_H__

#include "planet_solar_system.hpp"

#define SATURN_TEXTURE_FILENAME "../texture/saturn.jpg"

//===================================================================================
// CPlanetSaturn
//===================================================================================

class CPlanetSaturn
  : public CPlanetSolarSystem
{
public:
  CPlanetSaturn() : CPlanetSolarSystem() {}
  ~CPlanetSaturn(){};
  
  void Init(void);
  
  static const double RADIUS = 9.500000;
  
  static const int SLICES = 90;
  static const int STACKS = 45;
  
  static const double ROTATION_RATE = 810.792542;
  static const double ORBITAL_PERIOD = 10759.209961;
  static const double ASCENDING_NODE = 49.566399;
  static const double ORBIT_TILT = 2.485240;
  static const double PERIHELION = 88.863001;
  static const double MEAN_DISTANCE = 9.571900;
  static const double ECCENTRICITY = 0.053165;
  
};

#endif
