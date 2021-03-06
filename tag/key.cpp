#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <algorithm>
#include "key.h"

// キーボードdown関数
void Keyboard_Down(unsigned char key, int x, int y)
{
  switch(key){
  case ' ':// スペースキーが押されたら
    key_space = true;
    break;
  case 'k':
    key_k = true;
    break;
  case 'm':
    key_m=true;
    break;
  case '\33':      // ESCキーが押されたら
    exit(0);	// ウィンドウを閉じる
  default:
    break;
	}
}


// キーボードUP関数

void Keyboard_Up(unsigned char key, int x, int y)
{
  switch(key){
  case ' ':	// スペースキーを離したら
    key_space = false;
    break;
  case 'k':
    key_k=false;
    break;
  case 'm':
    key_m=false;
    break;
  default:
    break;
  }
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
}

void key_init(){
  
  //ボタンのフラグの初期化
  key_up    = false;
  key_down  = false;
  key_left  = false;
  key_right = false;
  key_space = false;
  key_k     = false;
  key_m     = false;
}
