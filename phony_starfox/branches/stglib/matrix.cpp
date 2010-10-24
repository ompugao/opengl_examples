/*
** 行列の計算
*/
#include <stdio.h>
#include <math.h>
#include "matrix.h"
#include <GL/glut.h>
/*
** 単位行列
*/
static const GLdouble ident[] = {
  1.0,  0.0,  0.0,  0.0,
  0.0,  1.0,  0.0,  0.0,
  0.0,  0.0,  1.0,  0.0,
  0.0,  0.0,  0.0,  1.0,
};

/*
** 変換行列のコピー（m1→m2）
*/
void copyMatrix(const GLdouble m1[], GLdouble m2[])
{
  int i;

  for (i = 0; i < 16; ++i) m2[i] = m1[i];
}

/*
** 単位行列を m に設定
*/
void loadIdentity(GLdouble m[])
{
  copyMatrix(ident, m);
}

/*
** 変換 (v1×m→v2)
*/
void transform(const GLdouble v1[], const GLdouble m[], GLdouble v2[])
{
  int i;
  GLdouble s[4];
  
  for (i = 0; i < 4; ++i) {
    s[i] = v1[0] * m[i] + v1[1] * m[i + 4] + v1[2] * m[i + 8] + v1[3] * m[i + 12];
  }
  
  for (i = 0; i < 4; ++i) v2[i] = s[i];
}

/*
** 行列の積 (m1×m2→m3)
*/
void multiply(const GLdouble m1[], const GLdouble m2[], GLdouble m3[])
{
  int j;
  
  for (j = 0; j < 4; ++j) {
    transform(m1, m2, m3);
    m1 += 4;
    m3 += 4;
  }
}

/*
** 平行移動
**
**   x, y, z: 移動量
**   m: 変換行列
*/
void translate(const GLdouble x, const GLdouble y, const GLdouble z, GLdouble t[])
{
  static GLdouble m[] = {
    1.0,  0.0,  0.0,  0.0,
    0.0,  1.0,  0.0,  0.0,
    0.0,  0.0,  1.0,  0.0,
    0.0,  0.0,  0.0,  1.0,
  };
  
  m[12] = x;
  m[13] = y;
  m[14] = z;
  
  multiply(t, m, t);
}

/*
** 任意軸周りの回転
**
**   a: 回転角
**   x, y, z: 軸
**   m: 変換行列
*/
void rotate(const GLdouble a, const GLdouble x, const GLdouble y, const GLdouble z, GLdouble r[])
{
  GLdouble d = x * x + y * y + z * z;
  
  if (d != 0.0) {
    GLdouble u  = sqrt(d);
    GLdouble l  = x / u, m  = y / u, n  = z / u;
    GLdouble l2 = l * l, m2 = m * m, n2 = n * n;
    GLdouble lm = l * m, mn = m * n, nl = n * l;
    GLdouble s = sin(a);
    GLdouble c = cos(a);
    GLdouble c1 = 1.0 - c;
    GLdouble t[16];
    
    t[ 0] = (1.0 - l2) * c + l2;
    t[ 1] = lm * c1 + n * s;
    t[ 2] = nl * c1 - m * s;
    
    t[ 4] = lm * c1 - n * s;
    t[ 5] = (1.0 - m2) * c + m2;
    t[ 6] = mn * c1 + l * s;
    
    t[ 8] = nl * c1 + m * s;
    t[ 9] = mn * c1 - l * s;
    t[10] = (1.0 - n2) * c + n2;
    
    t[ 3] = t[ 7] = t[11] = t[12] = t[13] = t[14] = 0.0;
    t[15] = 1.0;
    
    multiply(r, t, r);
  }
}

/*
** 逆行列の計算 (inv(m1)→m2)
*/
int inverse(const GLdouble m1[], GLdouble m2[])
{
  int i, j, k;
  GLdouble lu[20], *plu[4], det;
  
  /* j 行の要素の値の絶対値の最大値を plu[j][4] に求める */
  for (j = 0; j < 4; ++j) {
    GLdouble max = fabs(*(plu[j] = lu + 5 * j) = *(m1++));
    for (i = 0; ++i < 4;) {
      GLdouble a = fabs(plu[j][i] = *(m1++));
      if (a > max) max = a;
    }
    if (max == 0.0) return 0;
    plu[j][4] = 1.0 / max;
  }
  
  det = 1.0;
  
  /* ピボットを考慮した LU 分解 */
  for (j = 0; j < 4; ++j) {
    GLdouble max = fabs(plu[j][j] * plu[j][4]);
    i = j;
    for (k = j; ++k < 4;) {
      GLdouble a = fabs(plu[k][j] * plu[k][4]);
      if (a > max) {
        max = a;
        i = k;
      }
    }
    
    if (i > j) {
      GLdouble *t = plu[j];
      plu[j] = plu[i];
      plu[i] = t;
      det = -det;
    }
    if (plu[j][j] == 0.0) return 0;
    det *= plu[j][j];
    
    for (k = j; ++k < 4;) {
      plu[k][j] /= plu[j][j];
      for (i = j; ++i < 4;) {
        plu[k][i] -= plu[j][i] * plu[k][j];
      }
    }
  }
  
  /* 逆行列を求める */
  for (k = 0; k < 4; ++k) {
    /* m2 に単位行列を設定する */
    for (i = 0; i < 4; ++i) {
      m2[i * 4 + k] = (plu[i] == lu + k * 5) ? 1.0 : 0.0;
    }
    /* lu から逆行列を求める */
    for (i = 0; i < 4; ++i) {
      for (j = i; ++j < 4;) {
        m2[j * 4 + k] -= m2[i * 4 + k] * plu[j][i];
      }
    }
    for (i = 4; --i >= 0;){
      for (j = i; ++j < 4;) {
        m2[i * 4 + k] -= plu[i][j] * m2[j * 4 + k];
      }
      m2[i * 4 + k] /= plu[i][i];
    }
  }
  
  return -1;
}

/*
** 配列内容の表示
*/
void printMatrix(const GLdouble m[])
{
  int i;
  
  for (i = 0; i < 16; ++i) {
    printf("%8.4f", m[i]);
    if (i % 4 == 3) printf("\n");
  }
  printf("\n");
}

