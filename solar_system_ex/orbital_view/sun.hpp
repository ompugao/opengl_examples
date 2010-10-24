#ifndef __SUN_H__
#define __SUN_H__

#include "planet.hpp"

#define SUN_TEXTURE_FILENAME "../texture/sun.jpg"

//===================================================================================
// CSun
//===================================================================================

class CSun
  : public CPlanet
{
public:
  CSun() : CPlanet() {}
  ~CSun(){};
  
  void Init(void);
  void Draw(void);
  
  static const double RADIUS = 2.0;
  
  static const int SLICES = 90;
  static const int STACKS = 45;
  
  static const double ROTATION_RATE = 13.198;
  
  void setRenderTexture( bool val ) {m_b_texture = val; }
  void setRenderWire( bool val ) {m_b_wire = val; }
  
private:
  
  //
  bool m_b_texture;
  bool m_b_wire;
  
  
};

#endif
