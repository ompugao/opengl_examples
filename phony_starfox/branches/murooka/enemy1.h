#ifndef __ENEMY1_H__
#define __ENEMY1_H__

#include "vector.h"
#include "object.h"

class Enemy1 : public Object {
	void move();
public:
	Enemy1() {
		set_position(0, 0, 0);
		set_r(1);
		set_direction(0, 0);
	}
	Enemy1(double _x, double _y, double _z) {
		set_position(_x, _y, _z);
		set_r(1);
		set_direction(0, 0);
	}
	void draw();
};

extern Enemy1 enemy1;

#endif //__ENEMY1_H__
