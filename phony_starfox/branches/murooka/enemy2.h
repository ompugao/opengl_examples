#ifndef __ENEMY2_H__
#define __ENEMY2_H__

#include "vector.h"
#include "object.h"

class Enemy2 : public Object {
	void move();
public:
	Enemy2() {
		set_position(0, 0, 0);
		set_r(1);
		set_direction(0, 0);
	}
	Enemy2(double _x, double _y, double _z) {
		set_position(_x, _y, _z);
		set_r(1);
		set_direction(0, 0);
	}
	void draw();
};

extern Enemy2 enemy2;

#endif //__ENEMY2_H__
