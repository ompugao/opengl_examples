#ifndef _KEY_H_
#define _KEY_H_

//ボタンのフラグ
bool key_up;
bool key_down;
bool key_left;
bool key_right;
bool key_space;
bool key_k;
bool key_m;

void Keyboard_Down(unsigned char , int, int);
void Keyboard_Up(unsigned char key, int x, int y);
void Specialkey_Down(int key, int x, int y);
void Specialkey_Up(int key, int x, int y);
void key_init(void);


#endif // _KEY_H_
