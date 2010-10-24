MACRO(CREATE_LESSON NAME SOURCES)
	ADD_EXECUTABLE (${NAME} ${SOURCES} ${ARGN})
	IF(UNIX)
		TARGET_LINK_LIBRARIES (${NAME} libGL.so libGLU.so libSDL.so)
	ELSE (UNIX)
		IF(WIN32)
			TARGET_LINK_LIBRARIES (${NAME} opengl32.lib glu32.lib sdl.lib)
		ENDIF(WIN32)
	ENDIF(UNIX)
ENDMACRO(CREATE_LESSON)

FIND_PATH(SDL_INCLUDE_PATH SDL/SDL.h
	/usr/include
	/usr/local/include)


INCLUDE_DIRECTORIES (
	../src
	${SDL_INCUDE_PATH}
)

SET(NEHE_SOURCES
	../src/basecode/lesson.cpp
	../src/basecode/window.cpp
	../src/basecode/GLee/GLee.cpp
)

CREATE_LESSON (lesson01 ../src/lesson01.cpp ../src/main.cpp ${NEHE_SOURCES})
