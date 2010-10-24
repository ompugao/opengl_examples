#include "enemy_round.h"
#include "time_counter.h"
#include <cmath>

vector<EnemyRound> enemy_rounds;
vector<EnemyRound>::iterator p_enemy_rounds;

EnemyRound::EnemyRound(GLdouble a, GLdouble b, GLdouble c, GLdouble i, GLdouble j, GLdouble k, GLdouble _radius){
  center = make_vector(a, b, c, i, j, k);
  R = 50;
  radius = _radius;
}
/*EnemyRound::EnemyRound(const EnemyRound &_enemy_round){
  radius = _enemy_round.radius;
  center = _enemy_round.center;
}*/

void EnemyRound::move(){
  pos.x = radius*cos(time_count/10.0)+center.x;
  pos.y = radius*sin(time_count/10.0)+center.y;
  pos.z = center.z;
}

void EnemyRound::detail(){
  glColor3d(1.0,0.5,0.0);
  glutSolidCube(R);
}

EnemyRound make_enemy_round(GLdouble _x, GLdouble _y, GLdouble _z, GLdouble _radius)
{
  EnemyRound tmp(_x, _y, _z, 0, 0, 0, _radius);
  return tmp;
}
