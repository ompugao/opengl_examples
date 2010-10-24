#ifndef __PLANET_URANUS_H__
#define __PLANET_URANUS_H__

#include "planet_solar_system.hpp"

#define URANUS_TEXTURE_FILENAME "../texture/uranus.jpg"

//===================================================================================
// CPlanetUranus
//===================================================================================

class CPlanetUranus
  : public CPlanetSolarSystem
{
public:
  CPlanetUranus() : CPlanetSolarSystem() {}
  ~CPlanetUranus(){};
  
  void Init(void);
  
  static const double RADIUS = 3.900000;
  
  static const int SLICES = 90;
  static const int STACKS = 45;
  
  static const double ROTATION_RATE = 501.162415;
  static const double ORBITAL_PERIOD = 30685.000000;
  static const double ASCENDING_NODE = 74.095402;
  static const double ORBIT_TILT = 0.773430;
  static const double PERIHELION = 175.680703;
  static const double MEAN_DISTANCE = 19.301810;
  static const double ECCENTRICITY = 0.042896;
  
};

#endif
