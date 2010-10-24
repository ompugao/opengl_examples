#include "vector.h"
#include <cmath>

using namespace std;

double VECTOR::size(VECTOR){
	return (double)(sqrt(x * x + y * y + z * z));
}

// +演算子オーバーロード
VECTOR VECTOR::operator +(VECTOR A) {
	VECTOR C;
	C.x = x + A.x;
	C.y = y + A.y;
	C.z = z + A.z;
	return C;
}

// -演算子オーバーロード
VECTOR VECTOR::operator -(VECTOR A) {
	VECTOR C;
	C.x = x - A.x;
	C.y = y - A.y;
	C.z = z - A.z;
	return C;
}

// *演算子オーバーロード
double VECTOR::operator *(VECTOR A) {
	return x * A.x + y * A.y + z * A.z;
}

const VECTOR ZERO_VECTOR;
