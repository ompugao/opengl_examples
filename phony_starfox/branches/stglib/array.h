#ifndef __ARRAY_H__
#define __ARRAY_H__

/*
** マジックナンバー
*/

extern const double NULL_NUM;

/*
** 角度変換(弧度法から度数法へ)
*/

extern const double RAD2DEG;

/*
** 配列計算(3要素に限る)
*/

extern double *make_array(double _a, double _b, double _c);
extern double *subst_array(double *_a, double *_b);
extern double size_array(double *_a);

#endif //__ARRAY_H__
