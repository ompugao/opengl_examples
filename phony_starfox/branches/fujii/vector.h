/*
 *ベクトルの定義
 *座標もこれで記述する
 */
#include<GL/glut.h>

#ifndef __VECTOR_H__
#define __VECTOR_H__

class VECTOR {
 public:
  GLdouble x, y, z, theta1, theta2, theta3;
  VECTOR(GLdouble a=0,GLdouble b=0,GLdouble c=0,GLdouble i=0,GLdouble j=0,GLdouble k=0){
    x=a; y=b; z=c;
    theta1=i; theta2=j; theta3=k;
  }
  friend VECTOR operator+(VECTOR &ob1,VECTOR &ob2);
  friend VECTOR operator-(VECTOR &ob1,VECTOR &ob2);
  VECTOR operator-(void);
  friend double inner_product(VECTOR &ob1,VECTOR &ob2);
  //  friend VECTOR outer_product(VECTOR &ob1,VECTOR &ob2);
  void normalize(void);
  double size(void);
};

#endif // __VECTOR_H__
