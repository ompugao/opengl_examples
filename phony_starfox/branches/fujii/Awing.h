/*
 * 自機のクラス
 */
#ifndef __AWING_H__
#define __AWING_H__

#include "vector.h"
#include <GL/glut.h>

class Awing {
  VECTOR pos; //位置 position
  GLdouble *coord;//coordinate:座標　前回のローカル座標系を保存
  int speed;    //スピード
  double R;   //衝突検知用の球の半径
  void move();//位置移動、座標変換
  //void move_to_next_pos();
  void detail();
  void renew_pos();
 public:
  Awing();
  ~Awing();
  void draw(void);
  VECTOR get_pos(void);//位置を返す
  double get_R(void);  //衝突検知用の球の半径を返す
  //void collide();
};

#endif //__AWING_H__
