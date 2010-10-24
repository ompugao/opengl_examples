#ifndef __SELF_H__
#define __SELF_H__

#include "vector.h"
#include "object.h"

class Self : public Object {
	void move();
public:
	Self() {
		set_position(0, 0, 0);
		set_r(1);
		set_direction(0, 0);
	}
	Self(double _x, double _y, double _z) {
		set_position(_x, _y, _z);
		set_r(1);
		set_direction(0, 0);
	}
	void draw();
};

extern Self self;

#endif //__SELF_H__
