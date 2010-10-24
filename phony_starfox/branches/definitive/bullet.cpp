#include "bullet.h"
#include "key.h"
#include "enemy_round.h"
#include "vector.h"
#include "ground.h"
#include <cmath>
#include <iostream>

using namespace std;

vector<Bullet> bullets;
vector<Bullet>::iterator p_bullets;

Bullet::Bullet(GLdouble a, GLdouble b, GLdouble c, GLdouble i, GLdouble j, GLdouble k){
  pos = make_vector(a, b, c, i, j, k);
  R = 20;
}

Bullet::Bullet(GLdouble *_coord){
  copyMatrix(_coord, coord);
  R = 20;
}

Bullet::~Bullet(){
  //  delete [] coord;
}

void Bullet::move()
{
  glTranslated(100,0,0);
}

//点の配列
static GLdouble vertexdata[]={
  20.0, 0.0, 0.0,
  0.0, 0.0, 20.0,
  0.0, -20.0, 0.0,
  0.0, 0.0, -20.0,
  0.0, 20.0, 0.0,
  -50.0, 0.0, 0.0
};
//色の配列
static GLdouble colordata[]={
  0.0,1.0,0.0,
  0.0,0.7,0.7,
  0.0,0.7,0.7,
  0.0,0.7,0.7,
  0.0,0.7,0.7,
  0.0,1.0,1.0
};
//法線ベクトルの配列
static GLdouble normdata[]={
  1.0,-1.0,1.0,
  1.0,-1.0,-1.0,
  1.0,1.0,-1.0,
  1.0,1.0,1.0,
  -0.2,-0.5,0,5,
  -0.2,-0.5,-0.5,
  -0.2,0.5,-0.5,
  -0.2,0.5,0.5
};
//点のindex
static GLubyte vertexindices[]={
  1,2,3,  1,3,4, 1,4,5,  1,5,2,  6,3,2,  6,4,3, 6,5,4, 6,2,5
  //0,1,2,  0,2,3,  0,3,4,  0,4,1,  5,2,1,  5,3,2,  5,4,3,  5,1,4
  //あれぇ？
};


void Bullet::detail()
{
  if(abs((*p_bullets).get_pos().x) > FieldSize / 2 || 
     abs((*p_bullets).get_pos().y) > FieldSize / 2 || 
     abs((*p_bullets).get_pos().z - FieldSize / 4) > FieldSize / 4) {
     p_bullets = bullets.erase(p_bullets);
     return;
  } else {
    //glColor3f(1.0,0.0,0.0);
    /*
    glPushMatrix();
    glutSolidSphere(R, 10, 10);
    glPopMatrix();
    */
    GLfloat ambient[]={1.0,0.0,0.0,1.0};
    GLfloat specular[]={1.0,0.5,0.0,1.0};
    GLfloat shininess[]={128.0};
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,ambient);
    glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,shininess);

    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3,GL_DOUBLE,0,vertexdata);
    glColorPointer(3,GL_DOUBLE,0,colordata);
    glNormalPointer(GL_DOUBLE,0,normdata);
    glDrawElements(GL_TRIANGLES,8,GL_UNSIGNED_BYTE,vertexindices);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);


    collide();
    p_bullets++;
  }
}

int Bullet::collide()
{
  for(p_enemy_rounds = enemy_rounds.begin(); p_enemy_rounds != enemy_rounds.end(); p_enemy_rounds++) {
    if((pos - (*p_enemy_rounds).get_pos()).size() < ((*p_enemy_rounds).get_R() + R)) {
    cout << "You won!" << endl;
      exit(0);
      return 1;
    }
  }
  return 0;
}

Bullet make_bullet(GLdouble *_coord)
{
  Bullet tmp(_coord);
  return tmp;
}
