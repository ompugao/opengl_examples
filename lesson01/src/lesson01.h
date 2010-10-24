#ifndef _NEHE_LESSON_1
#define _NEHE_LESSON_1

#include "basecode/lesson.h"

class Lesson01 : public NeHe::Lesson
{
	private:
		virtual void draw();
		
	public:
		Lesson01();
		virtual bool init();
};

#endif
