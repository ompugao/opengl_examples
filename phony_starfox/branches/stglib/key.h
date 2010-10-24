#ifndef _KEY_H_
#define _KEY_H_

//ボタンのフラグ
extern bool key_up;
extern bool key_down;
extern bool key_left;
extern bool key_right;
extern bool key_space;
extern bool key_z;
extern bool key_x;
extern bool key_c;
extern bool key_v;
extern bool key_b;

void Keyboard_Down(unsigned char , int, int);
void Keyboard_Up(unsigned char key, int x, int y);
void Specialkey_Down(int key, int x, int y);
void Specialkey_Up(int key, int x, int y);
void key_init(void);


#endif // _KEY_H_
