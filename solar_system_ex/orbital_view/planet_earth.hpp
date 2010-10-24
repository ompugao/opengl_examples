#ifndef __PLANET_EARTH_H__
#define __PLANET_EARTH_H__

#include "planet_solar_system.hpp"

#define EARTH_TEXTURE_FILENAME "../texture/earthclouds_1024.jpg"

//===================================================================================
// CPlanetEarth
//===================================================================================

class CPlanetEarth
  : public CPlanetSolarSystem
{
public:
  CPlanetEarth() : CPlanetSolarSystem() {}
  ~CPlanetEarth(){};
  
  void Init(void);
  
  static const double RADIUS = 1.0;
  
  static const int SLICES = 90;
  static const int STACKS = 45;
  
  static const double ROTATION_RATE = 360.252502;
  static const double ORBITAL_PERIOD = 365.256012;
  static const double ASCENDING_NODE = 349.200006;
  static const double ORBIT_TILT = 0.000410;
  static const double PERIHELION = 102.851700;
  static const double MEAN_DISTANCE = 1.000020;
  static const double ECCENTRICITY = 0.016697;
  
};

#endif
