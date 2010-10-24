#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class Rotate3D{
private:
  static GLdouble m_matrix[16];
  static double m_spin;
  static void Draw();
  static void Rotate(GLdouble, GLdouble, GLdouble);
  static void Display();
  static void SpinDisplay();
  static void Reshape(int, int);
  static void Keyboard(unsigned char, int , int);
public:
  Rotate3D() { }
  virtual ~Rotate3D() { }
  void Init();
};

GLdouble Rotate3D::m_matrix[16];
double Rotate3D::m_spin = 0.0;

void Rotate3D::Draw(void)
{
  glColor3d(0.5, 0.5, 0.5);
  glutSolidDodecahedron();
 
}
void Rotate3D::Rotate(GLdouble x, GLdouble y, GLdouble z)
{
  glTranslated(0.0, 0.0, -10.0);
  glRotated(x, 1.0, 0.0, 0.0);
  glRotated(y, 0.0, 1.0, 0.0);
  glRotated(z, 0.0, 0.0, 1.0);
  glTranslated(0.0, 0.0, 10.0);
}

void Rotate3D::Display(void)
{
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  Rotate(m_spin, m_spin, m_spin);
  glMultMatrixd(m_matrix);
  Draw();
  glGetDoublev(GL_MODELVIEW_MATRIX, m_matrix);
  glutSwapBuffers();
}

void Rotate3D::SpinDisplay(void)
{
  m_spin = 0.1;
  glutPostRedisplay();
}

void Rotate3D::Reshape(int w, int h)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -1.0, 1.0, 5.0, 100.0);
}

void Rotate3D::Keyboard(unsigned char key , int x, int y)
{
  switch(key){
	case 27:
	 exit(0);
	default:
		break;
	}
}

void Rotate3D::Init(void)
{
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(300, 300);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Rotate3D");
  glViewport(0, 0, 300, 300);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  glGetDoublev(GL_MODELVIEW_MATRIX, m_matrix);
  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Keyboard);
  glutIdleFunc(SpinDisplay);
}
 
int main(int argc, char** argv){
  glutInit(&argc, argv);
  Rotate3D rotate3d;
  rotate3d.Init();
  glutMainLoop();
  return 0;
}
