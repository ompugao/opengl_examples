#include <GL/glut.h>
#include <GL/glui.h>

#include <math.h>

#include "get_clock_now.h"

// 太陽
#include "sun.hpp"

// 太陽系の惑星
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
// グローバル変数
//-----------------------------------------------------------------------------------
// GLUTウィンドウ識別子
int main_window = 0;

// 視点情報
static double distance = 40.0, pitch = -20.0, yaw = 0.0; 

// マウス入力情報
GLint mouse_button = -1;
GLint mouse_x = 0, mouse_y = 0;

// GLUI
GLUI *glui;

// GLUIユーザインタフェース
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

// GLUIのLive Variables
// 日の加算速度
static float day_rate = 0.1;

// 各半径の変数
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

// フラグ変数
// テクスチャをつけるか
static int is_texture = 1;

// ワイヤーフレーム表示にするか
static int is_wire = 0;

// 軌道を描くか
static int is_orbit = 1;

// GLUIコールバック用の識別子
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
  
// クロック
static double clock_now, clock_pre;

// 日
static double day = 0;

// デフォルトライト
static const GLfloat light_position[] = { -1.0, 1.0, 1.0, 0.0 };
static const GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1.0};
static const GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};

// デフォルトマテリアル
static const GLfloat mat_default_color[] = { 1.0, 1.0, 1.0, 1.0 };
static const GLfloat mat_default_specular[] = { 0.0, 0.0, 0.0, 0.0 };
static const GLfloat mat_default_shininess[] = { 100.0 };
static const GLfloat mat_default_emission[] = {0.0, 0.0, 0.0, 0.0};

// 太陽
CSun sun;

// 惑星
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
// GLUIのコールバック関数
//-----------------------------------------------------------------------------------
void control_cb( int control )
{
  switch(control) {
  case ID_CHECKBOX_TEXTURE:
    
    // 課題
    // 各planetのレンダリングにおける、テクスチャの使用を切替える
    // planet.hpp及びplanet_solar_system.hpp及びplanet_*.hppを参考に。    
    
    break;
    
  case ID_CHECKBOX_WIRE:
    
    // 課題
    // 各planetのレンダリングにおける、WIREFRAME表示とSOLID表示を切替える
    // planet.hpp及びplanet_solar_system.hpp及びplanet_*.hppを参考に。    
    
    break;
    
  case ID_CHECKBOX_ORBIT:
    
    // 課題
    // 各planetのレンダリングにおける、軌道の表示を切替える
    // planet.hpp及びplanet_solar_system.hpp及びplanet_*.hppを参考に。    
    
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
    
    // 課題
    // 各planetの半径を変える
    // planet.hpp及びplanet_solar_system.hpp及びplanet_*.hppを参考に。    
    
  }
}

//-----------------------------------------------------------------------------------
// ユーザインタフェース初期化
//-----------------------------------------------------------------------------------
void initUI(void)
{
  /*課題*/ = GLUI_Master.create_glui_subwindow( /*課題*/, 
					    GLUI_SUBWINDOW_RIGHT );
  
  ui_panel_controller = glui->add_panel( "Control" );
  
  ui_check_texture = glui->add_checkbox_to_panel( ui_panel_controller, "Texture", &is_texture, ID_CHECKBOX_TEXTURE, control_cb );
  ui_check_wire = glui->add_checkbox_to_panel( ui_panel_controller, "Wire", &is_wire, ID_CHECKBOX_WIRE, control_cb );
  ui_check_orbit = glui->add_checkbox_to_panel( ui_panel_controller, "Orbit", &is_orbit, ID_CHECKBOX_ORBIT, control_cb );
  
  glui->add_separator_to_panel( ui_panel_controller );
  
  // 課題
  // ui_spin_day_rateに、
  // ui_panel_controllerを親として、"Day Rate (day/second)"のラベルを持つGLUI_Spinnerを作る
  
  ui_spin_day_rate->set_float_limits( 0.01, 10.0 );
  ui_spin_day_rate->set_speed( 0.1 );
  
  ui_panel_radius = glui->add_panel( "Radius" );
  
  // 課題
  // 各planetについて
  // ui_spin_*_radiusに、
  // ui_panel_radiusを親として、惑星の名前(英語)のラベルを持つGLUI_Spinnerを作る
  
  // 次にset_float_limitsメソッドをつかって
  // 最小値0.1、最大値10.0に設定する。
  
  // またset_speedメソッドにより、
  // Spinnerの速度を0.5に設定する。
  
  glui->set_main_gfx_window( /*課題*/ );
}

//-----------------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------------
void init(void) 
{
  // クリアの値の設定
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glClearDepth( 1.0 );
  
  // デプステストを行う
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LESS );
  
  glShadeModel (GL_SMOOTH);
  
  // デフォルトライト
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  
  // デフォルトマテリアル
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_default_color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_default_color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_default_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_default_shininess);
  
  // 課題
  // 各CPlanetのクラスの初期化を行う
  // planet.hpp及びplanet_solar_system.hpp及びplanet_*.hppを参考に。    
  
  // クロックの初期化
  clock_now = clock_pre = get_clock_now();

}

//-----------------------------------------------------------------------------------
// レンダリング
//-----------------------------------------------------------------------------------
void display(void)
{
  // クロックの更新
  clock_pre = clock_now;
  clock_now = get_clock_now();
  
  // デフォルトでは10秒で1日分動くとする
  day += (clock_now - clock_pre) * day_rate;
  
  // 課題
  // 各CPlanetのクラスに"日"の設定を行う
  // planet.hpp及びplanet_solar_system.hpp及びplanet_*.hppを参考に。    
  
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  // 視点の設定
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotatef( 90.0, 0.0, 0.0, 1.0 );
  glTranslatef( 0.0, 0.0, -distance );
  glRotatef( -pitch, 0.0, -1.0, 0.0 );
  glRotatef( -yaw, 1.0, 0.0, 0.0 );
  
  // 課題
  // 各CPlanetのクラスにより描画を行う
  // planet.hpp及びplanet_solar_system.hpp及びplanet_*.hppを参考に。    
  
  glFlush();
  glutSwapBuffers(); 
}

//-----------------------------------------------------------------------------------
// ウィンドウリサイズのコールバック関数
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
// キーボード入力のコールバック関数
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
// マウスクリックのコールバック関数
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
// マウス移動のコールバック関数
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
// アイドル時のコールバック関数
//-----------------------------------------------------------------------------------
void idle()
{
  glutSetWindow(main_window);
  glutPostRedisplay();
}


//-----------------------------------------------------------------------------------
// メイン関数
//-----------------------------------------------------------------------------------
int main(int argc, char** argv)
{
  // GLUTの初期化
  glutInit(&argc, argv);
  glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
  glutInitWindowSize (1024, 480); 
  glutInitWindowPosition (50, 50);
  main_window = glutCreateWindow ("Orbital View");
  
  // UIの初期化
  initUI();
  
  // コールバック関数の設定
  glutDisplayFunc( display );
  GLUI_Master.set_glutReshapeFunc( reshape );  
  GLUI_Master.set_glutKeyboardFunc( keyboard );
  GLUI_Master.set_glutSpecialFunc( NULL );
  GLUI_Master.set_glutMouseFunc( mouse );
  glutMotionFunc( motion );
  GLUI_Master.set_glutIdleFunc( idle );
  
  // 初期化
  init();
  
  // メインループ
  glutMainLoop();
   
  return 0;
}
