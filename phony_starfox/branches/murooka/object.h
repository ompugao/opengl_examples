#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "vector.h"
#include <cmath>

extern const double RADIAN;

class Object {
protected:
	VECTOR position;
	VECTOR direction;
	VECTOR up;
	VECTOR v;
	double h_angle;
	double v_angle;
	double h_omega;
	double v_omega;
	double r;
	virtual void move() = 0;
	void set_position(double, double, double);
	void set_direction(double, double);
	void set_r(double);
public:
	Object() {
		set_position(0, 0, 0);
		set_r(10);
		set_direction(0, 0);
	}
	VECTOR get_pos();
	double get_r();
	virtual void draw() = 0;
};

#endif //__OBJECT_H__
