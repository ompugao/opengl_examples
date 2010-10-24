#include <GL/glut.h>
#include "key.h"

bool key_up;
bool key_down;
bool key_left;
bool key_right;
bool key_space;
bool key_f;
bool key_b;
bool key_q;
bool key_s;
bool key_c;
bool key_d;
bool key_ctrl;
bool key_alt;
bool key_shift;


// Ctrl, Alt, Shiftのフラグを更新する。全てのキーボードコールバック関数内で呼ばれる必要がある
// これらのキーは普通のキーと同時に使うべからず。SpecialキーならOK
static void my_get_modifiers();

// キーボードdown関数
void Keyboard_Down(unsigned char key, int x, int y)
{
  switch(key){
  case ' ':// スペースキーが押されたら
    key_space = true;
    break;
  case 'f':
    key_f = true;
    break;
  case 'b':
    key_b = true;
    break;
  case 'q':
    key_q = true;
    break;
  case 's':
    key_s = true;
    break;
  case 'c':
    key_c = true;
    break;
  case 'd':
    key_d = true;
    break;
  case '\33':      // ESCキーが押されたら
    exit(0);	// ウィンドウを閉じる
  default:
    break;
	}

	my_get_modifiers();
}


// キーボードUP関数

void Keyboard_Up(unsigned char key, int x, int y)
{
  switch(key){
  case ' ':	// スペースキーを離したら
    key_space = false;
    break;
  case 'f':
    key_f = false;
    break;
  case 'b':
    key_b = false;
    break;
  case 'q':
    key_q = false;
    break;
  case 's':
    key_s = false;
    break;
  case 'c':
    key_c = false;
    break;
  case 'd':
    key_d = false;
    break;
  default:
    break;
  }

	my_get_modifiers();
}


//特殊キーdown関数

void Specialkey_Down(int key, int x, int y)
{
  switch(key){
  case GLUT_KEY_UP:	// 上矢印キーキーが押されたら
    key_up = true;
    break;
  case GLUT_KEY_DOWN:	// 下矢印キーが押されたら
    key_down = true;
    break;
  case GLUT_KEY_LEFT:	// 左矢印キーが押されたら
    key_left = true;
    break;
  case GLUT_KEY_RIGHT:// 右矢印キーが押されたら
    key_right = true;
    break;
  default:
    break;
  }

	my_get_modifiers();
}


// 特殊キーup関数

void Specialkey_Up(int key, int x, int y)
{
  switch(key){
  case GLUT_KEY_UP:	// 上矢印キーを離したら
    key_up = false;
    break;
  case GLUT_KEY_DOWN:	// 下矢印キーを離したら
    key_down = false;
    break;
  case GLUT_KEY_LEFT:	// 左矢印キーを離したら
    key_left = false;
    break;
  case GLUT_KEY_RIGHT:// 右矢印キーを離したら
    key_right = false;
    break;
  default:
    break;
  }

	my_get_modifiers();
}

void my_get_modifiers()
{
	if(glutGetModifiers() == GLUT_ACTIVE_CTRL) {
		key_ctrl = true;
	} else {
		key_ctrl = false;
	}
	if(glutGetModifiers() == GLUT_ACTIVE_ALT) {
		key_alt = true;
	} else {
		key_alt = false;
	}
	if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
		key_shift = true;
	} else {
		key_shift = false;
	}
}

void key_init(){
  
  //ボタンのフラグの初期化
  key_up    = false;
  key_down  = false;
  key_left  = false;
  key_right = false;
  key_space = false;
  key_f     = false;
  key_b     = false;
  key_q     = false;
  key_s     = false;
  key_c     = false;
  key_d     = false;
  key_ctrl = false;
  key_alt = false;
  key_shift = false;
}
