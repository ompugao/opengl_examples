#include <GL/glut.h>
#include "display.h"
#include "camera.h"
#include "ground.h"
#include "self.h"
#include "enemy_round.h"
#include "bullet.h"

Self self(1000,1000,500);
Camera camera(1000,1000,500);
//注：やっぱりcamera.cppでself.hをincludeしても初期位置は渡せなかったorz
Ground ground;

//すべてのオブジェクトを描画
void AllDisplay(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  glLoadIdentity();

  camera.set_camera();

  //ここに描画するオブジェクトを登録していく
    ground.draw();
    self.draw();
    for(p_enemy_rounds = enemy_rounds.begin(); p_enemy_rounds != enemy_rounds.end(); p_enemy_rounds++) {
      (*p_enemy_rounds).draw();
    }
    for(p_bullets = bullets.begin(); p_bullets != bullets.end();) {
      (*p_bullets).draw();
    }
    
  //ここまで登録箇所
  glutSwapBuffers();
}

