#ifndef __SELF_H__
#define __SELF_H__

#include "local_object.h"

class Self : public LocalObject {
 protected:
  void move();//posを直接書き換え

  //フィールドの端へ行くと反対側へ行くようにするため、
  //オーバーロードする
  void post_move();

  //モノの形を記述
  void detail();
  // 当たり判定
  int collide();
 public:
  Self(GLdouble a = 0, GLdouble b = 0, GLdouble c = 0, GLdouble i = 0, GLdouble j = 0, GLdouble k = 0);
  ~Self();
};

extern Self self;

#endif // __SELF_H__
