#ifndef __BULLET_H__
#define __BULLET_H__

#include "local_object.h"
#include <vector>
using namespace std;

class Bullet : public LocalObject {
 protected:
  void move();//posを直接書き換え
  //モノの形を記述
  void detail();
  // 当たり判定
  int collide();
 public:
  Bullet(GLdouble a = 0, GLdouble b = 0, GLdouble c = 0, GLdouble i = 0, GLdouble j = 0, GLdouble k = 0);
  Bullet(GLdouble *_coord);
  ~Bullet();
};

extern vector<Bullet> bullets;
extern vector<Bullet>::iterator p_bullets;

Bullet make_bullet(GLdouble *coord);

#endif // __BULLET_H__
