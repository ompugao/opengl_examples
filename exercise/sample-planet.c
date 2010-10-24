/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*
 *  planet.c
 *  This program shows how to composite modeling transformations
 *  to draw translated and rotated models.
 *  Interaction:  pressing the d and y keys (day and year)
 *  alters the rotation of the planet around the sun.
 */
#include <GL/glut.h>
#include <stdlib.h>

static int year = 0, day = 0, moon_y = 0, moon_d = 0;

void init(void) 
{
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_FLAT);
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST); //なぜかdisplay()の中でenableしないとうまくいかない。
  glColor3f (1.0, 1.0, 1.0);

  glPushMatrix();
  glColor3f(1.0, 0.1, 0.0);
  glutSolidSphere(1.0, 20, 16);   /* draw sun */

  glRotatef ((GLfloat) year, 0.0, 1.0, 0.0);
  glTranslatef (2.0, 0.0, 0.0);
  glRotatef ((GLfloat) day, 0.0, 1.0, 0.0);
  glColor3f(0.0, 0.3, 0.8);
  glutSolidSphere(0.2, 10, 8);    /* draw smaller planet */
  
  glRotatef((GLfloat) moon_y, 0.0, 1.0, 0.0);
  glTranslatef(1.0, 0.0, 0.0);
  glRotatef((GLfloat) moon_d, 0.0, 1.0, 0.0);
  glColor3f(0.0, 1.0, 0.0);
  glutSolidSphere(0.1, 10, 8 );
  glPopMatrix();
  glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt (3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

/* ARGSUSED1 */
void keyboard (unsigned char key, int x, int y)
{
  switch (key) {
   case 'd':
     day = (day + 10) % 360;
     glutPostRedisplay();
     break;
   case 'D':
     day = (day - 10) % 360;
     glutPostRedisplay();
     break;
   case 'y':
     year = (year + 5) % 360;
     glutPostRedisplay();
     break;
cpp   case 'Y':
     year = (year - 5) % 360;
     glutPostRedisplay();
     break;
   case 'm':
     moon_y = (moon_y + 5) % 360;
     glutPostRedisplay();
     break;
   case 'M':
     moon_y = (moon_y - 5) % 360;
     glutPostRedisplay();
     break;
   case 'n':
     moon_d = (moon_d + 5) % 360;
     glutPostRedisplay();
     break;
   case 'N':
     moon_d = (moon_d - 5) % 360;
     glutPostRedisplay();
     break;    
   case 27:
     exit(0);
     break;
   default:
     break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
   glEnable(GL_DEPTH_TEST);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   //glutTimerFunc(50,timer,0);
   glutMainLoop();
   return 0;
}
