#ifndef __GROUND_H__
#define __GROUND_H__

#include "vector.h"
#include "object.h"

class Ground : public Object {
	VECTOR point[4];
	void move() { return; };
public:
	Ground() {
		VECTOR tmp0(-500, -500, 0);
		VECTOR tmp1(-500, 500, 0);
		VECTOR tmp2(500, 500, 0);
		VECTOR tmp3(500, -500, 0);		
		point[0] = tmp0;
		point[1] = tmp1;
		point[2] = tmp2;
		point[3] = tmp3;
		r = 0;
	}
	VECTOR get_pos();
	double get_r();
	void draw();
};

extern Ground ground;

#endif //__GROUND_H__
