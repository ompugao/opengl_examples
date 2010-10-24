#include <GL/glut.h>
#include <GL/glui.h>

#include <math.h>

#include "get_clock_now.h"

// ����
#include "sun.hpp"

// ���۷Ϥ�����
#include "planet_earth.hpp"
#include "planet_mercury.hpp"
#include "planet_venus.hpp"
#include "planet_mars.hpp"
#include "planet_jupiter.hpp"
#include "planet_saturn.hpp"
#include "planet_uranus.hpp"
#include "planet_neptune.hpp"
#include "planet_pluto.hpp"

//-----------------------------------------------------------------------------------
// �����Х��ѿ�
//-----------------------------------------------------------------------------------
// GLUT������ɥ����̻�
int main_window = 0;

// ��������
static double distance = 40.0, pitch = -20.0, yaw = 0.0; 

// �ޥ������Ͼ���
GLint mouse_button = -1;
GLint mouse_x = 0, mouse_y = 0;

// GLUI
GLUI *glui;

// GLUI�桼�����󥿥ե�����
GLUI_Panel* ui_panel_controller;

GLUI_Checkbox* ui_check_texture;
GLUI_Checkbox* ui_check_wire;
GLUI_Checkbox* ui_check_orbit;

GLUI_Spinner* ui_spin_day_rate;

GLUI_Panel* ui_panel_radius;
GLUI_Spinner* ui_spin_sun_radius;
GLUI_Spinner* ui_spin_earth_radius;
GLUI_Spinner* ui_spin_mercury_radius;
GLUI_Spinner* ui_spin_venus_radius;
GLUI_Spinner* ui_spin_mars_radius;
GLUI_Spinner* ui_spin_jupiter_radius;
GLUI_Spinner* ui_spin_saturn_radius;
GLUI_Spinner* ui_spin_uranus_radius;
GLUI_Spinner* ui_spin_neptune_radius;
GLUI_Spinner* ui_spin_pluto_radius;

// GLUI��Live Variables
// ���βû�®��
static float day_rate = 0.1;

// ��Ⱦ�¤��ѿ�
static float sun_radius = CSun::RADIUS;
static float earth_radius = CPlanetEarth::RADIUS;
static float mercury_radius = CPlanetMercury::RADIUS;
static float venus_radius = CPlanetVenus::RADIUS;
static float mars_radius = CPlanetMars::RADIUS;
static float jupiter_radius = CPlanetJupiter::RADIUS;
static float saturn_radius = CPlanetSaturn::RADIUS;
static float uranus_radius = CPlanetUranus::RADIUS;
static float neptune_radius = CPlanetNeptune::RADIUS;
static float pluto_radius = CPlanetPluto::RADIUS;

// �ե饰�ѿ�
// �ƥ��������Ĥ��뤫
static int is_texture = 1;

// �磻�䡼�ե졼��ɽ���ˤ��뤫
static int is_wire = 0;

// ��ƻ��������
static int is_orbit = 1;

// GLUI������Хå��Ѥμ��̻�
enum {
  ID_CHECKBOX_TEXTURE,
  ID_CHECKBOX_WIRE,
  ID_CHECKBOX_ORBIT,
  ID_SPINNER_SUN_RADIUS,
  ID_SPINNER_EARTH_RADIUS,
  ID_SPINNER_MERCURY_RADIUS,
  ID_SPINNER_VENUS_RADIUS,
  ID_SPINNER_MARS_RADIUS,
  ID_SPINNER_JUPITER_RADIUS,
  ID_SPINNER_SATURN_RADIUS,
  ID_SPINNER_URANUS_RADIUS,
  ID_SPINNER_NEPTUNE_RADIUS,
  ID_SPINNER_PLUTO_RADIUS  
};
  
// ����å�
static double clock_now, clock_pre;

// ��
static double day = 0;

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
CSun sun;

// ����
CPlanetEarth earth;
CPlanetMercury mercury;
CPlanetVenus venus;
CPlanetMars mars;
CPlanetJupiter jupiter;
CPlanetSaturn saturn;
CPlanetUranus uranus;
CPlanetNeptune neptune;
CPlanetPluto pluto;

//-----------------------------------------------------------------------------------
// GLUI�Υ�����Хå��ؿ�
//-----------------------------------------------------------------------------------
void control_cb( int control )
{
  switch(control) {
  case ID_CHECKBOX_TEXTURE:
    
    // ����
    // ��planet�Υ�����󥰤ˤ����롢�ƥ�������λ��Ѥ����ؤ���
    // planet.hpp�ڤ�planet_solar_system.hpp�ڤ�planet_*.hpp�򻲹ͤˡ�    
    
    break;
    
  case ID_CHECKBOX_WIRE:
    
    // ����
    // ��planet�Υ�����󥰤ˤ����롢WIREFRAMEɽ����SOLIDɽ�������ؤ���
    // planet.hpp�ڤ�planet_solar_system.hpp�ڤ�planet_*.hpp�򻲹ͤˡ�    
    
    break;
    
  case ID_CHECKBOX_ORBIT:
    
    // ����
    // ��planet�Υ�����󥰤ˤ����롢��ƻ��ɽ�������ؤ���
    // planet.hpp�ڤ�planet_solar_system.hpp�ڤ�planet_*.hpp�򻲹ͤˡ�    
    
    break;
    
  case ID_SPINNER_SUN_RADIUS:    
    break;
    
  case ID_SPINNER_EARTH_RADIUS:
    break;
    
  case ID_SPINNER_MERCURY_RADIUS:
    break;
    
  case ID_SPINNER_VENUS_RADIUS:
    break;
    
  case ID_SPINNER_MARS_RADIUS:
    break;
    
  case ID_SPINNER_JUPITER_RADIUS:
    break;
    
  case ID_SPINNER_SATURN_RADIUS:
    break;
    
  case ID_SPINNER_URANUS_RADIUS:
    break;
    
  case ID_SPINNER_NEPTUNE_RADIUS:
    break;

  case ID_SPINNER_PLUTO_RADIUS:
    break;
    
    // ����
    // ��planet��Ⱦ�¤��Ѥ���
    // planet.hpp�ڤ�planet_solar_system.hpp�ڤ�planet_*.hpp�򻲹ͤˡ�    
    
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
  
  ui_check_texture = glui->add_checkbox_to_panel( ui_panel_controller, "Texture", &is_texture, ID_CHECKBOX_TEXTURE, control_cb );
  ui_check_wire = glui->add_checkbox_to_panel( ui_panel_controller, "Wire", &is_wire, ID_CHECKBOX_WIRE, control_cb );
  ui_check_orbit = glui->add_checkbox_to_panel( ui_panel_controller, "Orbit", &is_orbit, ID_CHECKBOX_ORBIT, control_cb );
  
  glui->add_separator_to_panel( ui_panel_controller );
  
  // ����
  // ui_spin_day_rate�ˡ�
  // ui_panel_controller��ƤȤ��ơ�"Day Rate (day/second)"�Υ�٥�����GLUI_Spinner����
  
  ui_spin_day_rate->set_float_limits( 0.01, 10.0 );
  ui_spin_day_rate->set_speed( 0.1 );
  
  ui_panel_radius = glui->add_panel( "Radius" );
  
  // ����
  // ��planet�ˤĤ���
  // ui_spin_*_radius�ˡ�
  // ui_panel_radius��ƤȤ��ơ�������̾��(�Ѹ�)�Υ�٥�����GLUI_Spinner����
  
  // ����set_float_limits�᥽�åɤ�Ĥ��ä�
  // �Ǿ���0.1��������10.0�����ꤹ�롣
  
  // �ޤ�set_speed�᥽�åɤˤ�ꡢ
  // Spinner��®�٤�0.5�����ꤹ�롣
  
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
  // ��CPlanet�Υ��饹�ν������Ԥ�
  // planet.hpp�ڤ�planet_solar_system.hpp�ڤ�planet_*.hpp�򻲹ͤˡ�    
  
  // ����å��ν����
  clock_now = clock_pre = get_clock_now();

}

//-----------------------------------------------------------------------------------
// �������
//-----------------------------------------------------------------------------------
void display(void)
{
  // ����å��ι���
  clock_pre = clock_now;
  clock_now = get_clock_now();
  
  // �ǥե���ȤǤ�10�ä�1��ʬư���Ȥ���
  day += (clock_now - clock_pre) * day_rate;
  
  // ����
  // ��CPlanet�Υ��饹��"��"�������Ԥ�
  // planet.hpp�ڤ�planet_solar_system.hpp�ڤ�planet_*.hpp�򻲹ͤˡ�    
  
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  // ����������
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotatef( 90.0, 0.0, 0.0, 1.0 );
  glTranslatef( 0.0, 0.0, -distance );
  glRotatef( -pitch, 0.0, -1.0, 0.0 );
  glRotatef( -yaw, 1.0, 0.0, 0.0 );
  
  // ����
  // ��CPlanet�Υ��饹�ˤ�������Ԥ�
  // planet.hpp�ڤ�planet_solar_system.hpp�ڤ�planet_*.hpp�򻲹ͤˡ�    
  
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
  gluPerspective(60.0, (float)tw / (float)th, 0.5, 10000.0);
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
  mouse_button = button;
  mouse_x = x;	mouse_y = y;
  
  if(state == GLUT_UP){
    mouse_button = -1;
  }
  
  glutSetWindow(main_window);
  glutPostRedisplay();
}

//-----------------------------------------------------------------------------------
// �ޥ�����ư�Υ�����Хå��ؿ�
//-----------------------------------------------------------------------------------
void motion(int x, int y)
{
  switch(mouse_button){
  case GLUT_LEFT_BUTTON:
    
    if( x == mouse_x && y == mouse_y )
      return;
    
    yaw -= (GLfloat) (x - mouse_x) / 30.0;
    pitch -= (GLfloat) (y - mouse_y) / 30.0;
    
    if( yaw < 0.0 ) yaw += 360.0;
    if( yaw > 360.0 ) yaw -= 360.0;
    
    if( pitch < -90.0 ) pitch = -90.0;
    if( pitch > 90.0 ) pitch = 90.0;
    
    break;
    
  case GLUT_RIGHT_BUTTON:
    
    if( y == mouse_y )
      return;
    
    if( y < mouse_y )
      distance += (GLfloat) (mouse_y - y)/50.0 * sqrt(distance);
    else
      distance -= (GLfloat) (y-mouse_y)/50.0 * sqrt(distance);
    
    if( distance < 1.0 ) distance = 1.0;
    if( distance > 2000.0 ) distance = 2000.0;
    
    break;
  }
    
  mouse_x = x;
  mouse_y = y;
  
  glutSetWindow(main_window);
  glutPostRedisplay();
}

//-----------------------------------------------------------------------------------
// �����ɥ���Υ�����Хå��ؿ�
//-----------------------------------------------------------------------------------
void idle()
{
  glutSetWindow(main_window);
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
  glutInitWindowSize (1024, 480); 
  glutInitWindowPosition (50, 50);
  main_window = glutCreateWindow ("Orbital View");
  
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
