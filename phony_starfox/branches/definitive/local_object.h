/*
 * 前の座標系から考えるobject
 * 今はposのみ更新されるが、coord[16]から向きのベクトルも得られる
 */

#ifndef __LOCAL_OBJECT_H__
#define __LOCAL_OBJECT_H__

#include "vector.h"
#include "camera.h"
#include "matrix.h"
#include <GL/glut.h>

class LocalObject {
 protected:
  VECTOR pos; //位置 position
  GLdouble *coord;//coordinate:座標(カメラを考慮しない) ローカル座標系を保存する pos等を得られる
  //  GLdouble matrix[16];//カメラを考慮した座標変換行列(描画に使用)
  double R;   //衝突検知用の球の半径
  void pre_move();
  virtual void move()=0;//現周期での位置移動、座標変換
  virtual void post_move();

  virtual void detail()=0;
  void renew_pos();//pos更新
  void multiply_camera_matrix();//カメラの行列をかける
 public:
  LocalObject();
  LocalObject(const LocalObject &_local_object);
  virtual ~LocalObject();
  void draw(void);
  VECTOR get_pos(void);//位置を返す
  double get_R(void);  //衝突検知用の球の半径を返す
  GLdouble *get_coord(void);
};

#endif //__LOCAL_OBJECT_H__
