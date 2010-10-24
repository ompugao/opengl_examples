#include <GL/glut.h>
#include "self.h"
#include "key.h"

void Self::move() 
{
	if((!key_c) && (!key_d) && (!key_shift)) {
		if(key_f) { v.x = 15; } else if (key_b) { v.x = -15; } else { v.x = 0; }
		if(key_left) { v.y = 15; } else if (key_right) { v.y = -15; } else { v.y = 0; }
		if(key_up) { v.z = 15; } else if (key_down) { v.z = -15; } else { v.z = 0; }
		h_omega = 0;
		v_omega = 0;
	} else if ((!key_c) && (key_d) && (!key_shift)) {
		if(key_left) { h_omega = 5; } else if (key_right) { h_omega = -5; } else { h_omega = 0; }
		if(key_up) { v_omega = 5; } else if (key_down) { v_omega = -5; } else { v_omega = 0; }
		v = ZERO_VECTOR;
	} else {
		v = ZERO_VECTOR;
		h_omega = 0;
		v_omega = 0;
	}
	if(key_shift) {
		if(key_up) { r += 0.1; } else if (key_down) { r -= 0.1; }
	}
	
	position = position + v;
	h_angle += h_omega;
	v_angle += v_omega;
	set_direction(h_angle, v_angle);

	return;
}

void Self::draw()
{
	move();

	glPushMatrix();

	glTranslatef(position.x, position.y , position.z);
	glRotated(h_angle, 0, 0, 1);
	glRotated(v_angle, 0, 1, 0);
	glRotated(90, 1, 0, 0);

	glColor3f(1.0, 1.0, 0.3);
	glutSolidTeapot(50.0 * r);

	glPopMatrix();

	return;
}

Self self(0, 0, 100);

