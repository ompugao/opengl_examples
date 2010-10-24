#ifndef __WORLD_OBJECT_H__
#define __WORLD_OBJECT_H__

#include "vector.h"
#include "key.h"
#include "matrix.h"
#include "camera.h"
#include <GL/glut.h>

class WorldObject{
 protected:
  
  VECTOR pos; //位置 position
  GLdouble *coord;//coordinate:座標(カメラを考慮しない) ローカル座標系を保存する pos等を得られる
  GLdouble R;   //衝突検知用の球の半径
  
  void pre_move();
  virtual void move()=0;//posを直接書き換え
  void post_move();

  //モノの形を記述
  virtual void detail()=0;
 public:
  WorldObject();
  WorldObject(const WorldObject &_world_object);
  virtual ~WorldObject();
  void draw(void);
  VECTOR get_pos(void);//位置を返す
  double get_R(void);  //衝突検知用の球の半径を返す  
};

#endif // __WORLD_OBJECT_H__
