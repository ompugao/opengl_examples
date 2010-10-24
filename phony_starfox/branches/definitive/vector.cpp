#include "vector.h"
#include <cmath>
#include <algorithm>

VECTOR VECTOR::operator+(VECTOR ob2){
  VECTOR temp;
  temp.x = x + ob2.x;
  temp.y = y + ob2.y;
  temp.z = z + ob2.z;
  return temp;
}

VECTOR VECTOR::operator-(VECTOR ob2){
  VECTOR temp;
  temp.x = x - ob2.x;
  temp.y = y - ob2.y;
  temp.z = z - ob2.z;
  return temp;
}

VECTOR VECTOR::operator-(){
  x=-x;
  y=-y;
  z=-z;
  return *this;
}

double inner_product(VECTOR &ob1,VECTOR &ob2){
  return ob1.x*ob2.x + ob1.y*ob2.y + ob1.z*ob2.z ;
}
/*
VECTOR outer_product(VECTOR &ob1,VECTOR &ob2){
  return VECTOR((ob1.y*ob2.z-ob1.z*ob2.y),(ob1.z*ob2.x-ob1.x*ob2.z),(ob1.x*ob2.y-ob1.y*ob2.x));
}
*/
void VECTOR::normalize(void){
  double tmp_size=this->size();
  x=x/tmp_size;
  y=y/tmp_size;
  z=z/tmp_size;
}

double VECTOR::size(void){
  return sqrt(x*x + y*y + z*z);
}

VECTOR make_vector(GLdouble a,GLdouble b,GLdouble c,GLdouble i,GLdouble j,GLdouble k)
{
    VECTOR temp(a, b, c, i, j, k);
    return temp;
}
