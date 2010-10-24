#include <GL/glut.h>
#include <GL/glui.h>

#include "planet_custom.hpp"

//-----------------------------------------------------------------------------------
// �����Х��ѿ�
//-----------------------------------------------------------------------------------
// GLUT������ɥ����̻�
int main_window = 0;

// GLUI
GLUI *glui;

// GLUI�桼�����󥿥ե�����
GLUI_Panel* ui_panel_controller;

GLUI_Spinner* ui_spin_distance;
GLUI_Spinner* ui_spin_slices;
GLUI_Spinner* ui_spin_stacks;

GLUI_Checkbox* ui_check_wire;

GLUI_Rotation* ui_rot_rotation;

// GLUI��Live variable
// ��ž�ޥȥꥯ��
float rot[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

// �����ε�Υ
float distance = 5.0;

// planet�ǵ������ˤ�������ѿ�
int slices = CPlanetCustom::SLICES;
int stacks = CPlanetCustom::STACKS;

// �磻�䡼�ե졼���ɽ�����뤫���ʤ���
int is_wire = 0;

// GLUI������Хå��Ѥμ��̻�
enum {
  ID_SPINNER_SLICES,
  ID_SPINNER_STACKS,
  ID_CHECKBOX_WIRE
};

// �ǥե���ȥ饤��
static const GLfloat light_position[] = { -1.0, 1.0, 1.0, 0.0 };
static const GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1.0};
static const GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};

// �ǥե���ȥޥƥꥢ��
static const GLfloat mat_default_color[] = { 1.0, 1.0, 1.0, 1.0 };
static const GLfloat mat_default_specular[] = { 0.0, 0.0, 0.0, 0.0 };
static const GLfloat mat_default_shininess[] = { 100.0 };
static const GLfloat mat_default_emission[] = {0.0, 0.0, 0.0, 0.0};

// ����
CPlanetCustom planet;

//-----------------------------------------------------------------------------------
// GLUI�Υ�����Хå��ؿ�
//-----------------------------------------------------------------------------------
void control_cb( int control )
{
  switch( control ) {
  case ID_SPINNER_SLICES:
    
    // ����
    // planet��slices���ѹ�����
    // planet.hpp�ڤ�planet_custom.hpp�򻲹ͤˡ�    
    
    break;
    
  case ID_SPINNER_STACKS:
    
    // ����
    // planet��stacks���ѹ�����
    // planet.hpp�ڤ�planet_custom.hpp�򻲹ͤˡ�    
    
    break;
  
  case ID_CHECKBOX_WIRE:
    
    // ����
    // planet��WIREFRAMEɽ����SOLIDɽ�������ؤ���
    // planet.hpp�ڤ�planet_custom.hpp�򻲹ͤˡ�    
    break;
  }
}

//-----------------------------------------------------------------------------------
// �桼�����󥿥ե����������
//-----------------------------------------------------------------------------------
void initUI(void)
{
  /*����*/ = GLUI_Master.create_glui_subwindow( /*����*/,
						GLUI_SUBWINDOW_RIGHT );
  
  ui_panel_controller = glui->add_panel( "Control" );
  
  ui_spin_slices = glui->add_spinner_to_panel( ui_panel_controller, "Slices", GLUI_SPINNER_INT, /*����*/, ID_SPINNER_SLICES, /*����*/ );
  ui_spin_slices->set_int_limits( 3, 150 );
  ui_spin_slices->set_speed( 0.03 );
  
  ui_spin_stacks = glui->add_spinner_to_panel( ui_panel_controller, "Stacks", GLUI_SPINNER_INT, /*����*/, ID_SPINNER_STACKS, /*����*/ );
  ui_spin_stacks->set_int_limits( 3, 150 );
  ui_spin_stacks->set_speed( 0.03 );
  
  ui_check_wire = glui->add_checkbox_to_panel( ui_panel_controller, "Wireframe", /*����*/, ID_CHECKBOX_WIRE, /*����*/ );
  ui_check_wire->set_int_val( is_wire );
  
  glui->add_separator_to_panel( ui_panel_controller );
  
  ui_spin_distance = glui->add_spinner_to_panel( ui_panel_controller, "Distance", GLUI_SPINNER_FLOAT, /*����*/ );
  ui_spin_distance->set_float_limits( 1.5, 10.0 );
  ui_spin_distance->set_speed( 0.1 );
  
  ui_rot_rotation = glui->add_rotation_to_panel( ui_panel_controller, "Rotation", /*����*/ );
  
  glui->set_main_gfx_window( /*����*/ );
}

//-----------------------------------------------------------------------------------
// �����
//-----------------------------------------------------------------------------------
void init(void) 
{
  // ���ꥢ���ͤ�����
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glClearDepth( 1.0 );
  
  // �ǥץ��ƥ��Ȥ�Ԥ�
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LESS );
  
  glShadeModel (GL_SMOOTH);
  
  // �ǥե���ȥ饤��
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  
  // �ǥե���ȥޥƥꥢ��
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_default_color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_default_color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_default_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_default_shininess);
  
  // ����
  // CPlanetCustom���饹�ν������Ԥ�
  // planet.hpp�ڤ�planet_custom.hpp�򻲹ͤˡ�
  
}

//-----------------------------------------------------------------------------------
// �������
//-----------------------------------------------------------------------------------
void display(void)
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  // ����������
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef( 0.0, 0.0, -distance );
  glMultMatrixf( rot );
  
  // ����
  // CPlanetCustom���饹�ˤ�������������Ԥ���
  // planet.hpp�ڤ�planet_custom.hpp�򻲹ͤˡ�
  
  glFlush();
  glutSwapBuffers(); 
}

//-----------------------------------------------------------------------------------
// ������ɥ��ꥵ�����Υ�����Хå��ؿ�
//-----------------------------------------------------------------------------------
void reshape (int w, int h)
{
  int tx, ty, tw, th;
  GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
  glViewport( tx, ty, tw, th );
  
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(60.0, (float)tw / (float)th, 0.5, 20.0);
}

//-----------------------------------------------------------------------------------
// �����ܡ������ϤΥ�����Хå��ؿ�
//-----------------------------------------------------------------------------------
void keyboard (unsigned char key, int x, int y)
{
  switch (key) {
  case 27:
    exit(0);
    break;
  }
}

//-----------------------------------------------------------------------------------
// �ޥ�������å��Υ�����Хå��ؿ�
//-----------------------------------------------------------------------------------
void mouse(int button, int state, int x, int y)
{
  glutPostRedisplay();
}

//-----------------------------------------------------------------------------------
// �ޥ�����ư�Υ�����Хå��ؿ�
//-----------------------------------------------------------------------------------
void motion(int x, int y)
{
  glutPostRedisplay();
}

//-----------------------------------------------------------------------------------
// �����ɥ���Υ�����Хå��ؿ�
//-----------------------------------------------------------------------------------
void idle()
{
  glutPostRedisplay();
}


//-----------------------------------------------------------------------------------
// �ᥤ��ؿ�
//-----------------------------------------------------------------------------------
int main(int argc, char** argv)
{
  // GLUT�ν����
  glutInit(&argc, argv);
  glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
  glutInitWindowSize (640, 480); 
  glutInitWindowPosition (50, 50);
  main_window = glutCreateWindow ("Planet View");
  
  // UI�ν����
  initUI();
  
  // ������Хå��ؿ�������
  glutDisplayFunc( display );
  GLUI_Master.set_glutReshapeFunc( reshape );  
  GLUI_Master.set_glutKeyboardFunc( keyboard );
  GLUI_Master.set_glutSpecialFunc( NULL );
  GLUI_Master.set_glutMouseFunc( mouse );
  glutMotionFunc( motion );
  GLUI_Master.set_glutIdleFunc( idle );
  
  // �����
  init();
  
  // �ᥤ��롼��
  glutMainLoop();
   
  return 0;
}
