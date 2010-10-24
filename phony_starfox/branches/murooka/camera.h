#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "vector.h"
#include <cmath>

extern const double RADIAN;

class Camera {
	VECTOR position;
	VECTOR direction;
	VECTOR up;
	VECTOR v;
	double h_angle;
	double v_angle;
	double h_omega;
	double v_omega;
	void move();
public:
	Camera() {
		h_angle = 0 * RADIAN;
		v_angle = -25 * RADIAN;
		VECTOR init_position(-500, 0, 300);
		VECTOR init_direction(cos(h_angle), sin(h_angle), sin(v_angle));
		VECTOR init_up(0, 0, 1);
		position = init_position;
		direction = init_direction;
		up = init_up;
	}
	void set();
};

extern Camera camera;

#endif //__CAMERA_H__
