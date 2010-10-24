#include <GL/glut.h>
#include "camera.h"
#include "key.h"

Camera camera;

void Camera::move()
{
	if(key_c && (!key_shift) && (!key_d)) {
		if(key_f) { v.x = 15; } else if (key_b) { v.x = -15; } else { v.x = 0; }
		if(key_left) { v.y = 15; } else if (key_right) { v.y = -15; } else { v.y = 0; }
		if(key_up) { v.z = 15; } else if (key_down) { v.z = -15; } else { v.z = 0; }
	} else {
		v = ZERO_VECTOR;
	}
	if(key_c && (!key_shift) && key_d) {
		if(key_left) { h_omega = 5 * RADIAN; } else if (key_right) { h_omega = -5 * RADIAN; } else { h_omega = 0; }
		if(key_up) { v_omega = 5 * RADIAN; } else if (key_down) { v_omega = -5 * RADIAN; } else { v_omega = 0; }
	} else {
		v_omega = 0;
		h_omega = 0;
	}

	position = position + v;
	v_angle += v_omega;
	h_angle += h_omega;
	direction.x = cos(h_angle);
	direction.y = sin(h_angle);
	direction.z = sin(v_angle);

	return;
}

void Camera::set()
{
	VECTOR target;

	move();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	target = position + direction;
	gluLookAt(position.x, position.y, position.z, target.x, target.y, target.z, up.x, up.y, up.z);

	return;
}
