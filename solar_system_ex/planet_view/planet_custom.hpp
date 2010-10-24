#ifndef __PLANER_CUSTOM_H__
#define __PLANER_CUSTOM_H__

#include "planet.hpp"

//===================================================================================
// CPlanetCustom
//===================================================================================
class CPlanetCustom
  :public CPlanet
{
public:
  CPlanetCustom();
  ~CPlanetCustom() {}
  
  void Init(void);
  void Draw(void);
  
  static const double RADIUS = 1.0;
  
  static const int SLICES = 90;
  static const int STACKS = 45;

  void setRenderWire( bool val ) { m_b_wire = val; }
  
private:
  bool m_b_wire;
};

#endif
