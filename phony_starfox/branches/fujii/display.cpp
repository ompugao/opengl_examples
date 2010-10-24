#include <GL/glut.h>
#include "display.h"
#include "Awing.h"
#include "ground.h"
#include "camera.h"

Camera camera;
double camera_matrix[16];

//すべてのオブジェクトを描画
void AllDisplay(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  //camera設置、行列保存
  camera.set_camera();
  camera.move_camera();

  //ここに描画するオブジェクトを登録していく
  //jimen.draw();
  //Awing1.draw();
  //ここまで登録箇所

  glutSwapBuffers();
}

