#include <cmath>
#include "array.h"

/*
** マジックナンバー
*/

const double NULL_NUM = -10000;

/*
** 角度変換(弧度法から度数法へ)
*/

const double RAD2DEG = 180 / 3.14;

/*
** 配列を返す
*/
double *make_array(double _a, double _b, double _c)
{
    double *tmp;
    tmp = new double [3];
    tmp[0] = _a;
    tmp[1] = _b;
    tmp[2] = _c;
    return tmp;
}

/*
** 2つの配列の対応する要素を引く(_a - _b)
*/
double *subst_array(double *_a, double *_b)
{
    return make_array(_a[0] - _b[0], _a[1] - _b[1], _a[2] - _b[2]);
}

/*
** 配列の大きさ(3つの要素の自乗和)を返す
*/
double size_array(double *_a)
{
    return sqrt(_a[0] * _a[0] + _a[1] * _a[1] + _a[2] * _a[2]);
}

