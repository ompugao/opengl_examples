#include "lesson01.h"

Lesson01::Lesson01()
{
}

bool Lesson01::init()
{
	if (!m_Window.createWindow(800, 600, 32, false, "Ne(w)He Lesson 1 - The first OpenGL window")) 
	{
		return false;
	}
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	return true;
}

void Lesson01::draw()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}
