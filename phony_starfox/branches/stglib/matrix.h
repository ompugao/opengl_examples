#ifndef INC_MATRIX_H
#define INC_MATRIX_H
#include <GL/glut.h>

/*
** ¹ÔÎó·×»»
*/
extern void copyMatrix(const GLdouble m1[], GLdouble m2[]);
extern void loadIdentity(GLdouble m[]);
extern void transform(const GLdouble v1[], const GLdouble m[], GLdouble v2[]);
extern void multiply(const GLdouble m1[], const GLdouble m2[], GLdouble m3[]);
extern void translate(const GLdouble x, const GLdouble y, const GLdouble z, GLdouble t[]);
extern void rotate(const GLdouble a, const GLdouble x, const GLdouble y, const GLdouble z, GLdouble r[]);
extern int inverse(const GLdouble m1[], GLdouble m2[]);
extern void printMatrix(const GLdouble m[]);

#endif
