#ifndef __VECTOR_H__
#define __VECTOR_H__

// 3次元ベクトル
class VECTOR {
public:
	VECTOR() {
		x = 0;
		y = 0;
		z = 0;
	}
	VECTOR(double _x, double _y, double _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	double x; // x座標
	double y; // y座標
	double z; // z座標
	double size(VECTOR); // 大きさを返す
	VECTOR operator + (VECTOR); // +演算子オーバーロード
	VECTOR operator - (VECTOR); // -演算子オーバーロード
	double operator * (VECTOR); // *演算子オーバーロード(内積) 
};

extern const VECTOR ZERO_VECTOR;

#endif //__VECTOR_H__

