#ifndef __ENEMY_ROUND_H__
#define __ENEMY_ROUND_H__

#include "world_object.h"
#include <vector>

using namespace std;

class EnemyRound : public WorldObject{
 protected:
  GLdouble radius;
  VECTOR center;
  void move();
  void detail();
 public:
  EnemyRound(GLdouble a=0, GLdouble b=0, GLdouble c=0, GLdouble i=0, GLdouble j=0, GLdouble k=0, GLdouble _radius=50);
//  EnemyRound(const EnemyRound &_enemy_round);
};

extern vector<EnemyRound> enemy_rounds;
extern vector<EnemyRound>::iterator p_enemy_rounds;

EnemyRound make_enemy_round(GLdouble, GLdouble, GLdouble, GLdouble);

#endif // __ENEMY_ROUND_H__
