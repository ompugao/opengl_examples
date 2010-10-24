#include <GL/glut.h>
#include "display.h"

#include "camera.h"
#include "ground.h"
#include "self.h"
#include "enemy1.h"
#include "enemy2.h"

static void init_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

  glLoadIdentity();

	return;
}

//すべてのオブジェクトを描画
void AllDisplay(){

	init_display();

	camera.set();

  //ここに描画するオブジェクトを登録していく
	ground.draw();
	self.draw();
	enemy1.draw();
	enemy2.draw();

  //ここまで登録箇所
  glutSwapBuffers();
}

