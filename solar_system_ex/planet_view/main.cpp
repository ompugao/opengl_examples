#include <GL/glut.h>
#include <GL/glui.h>

#include "planet_custom.hpp"

//-----------------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------------
// GLUTウィンドウ識別子
int main_window = 0;

// GLUI
GLUI *glui;

// GLUIユーザインタフェース
GLUI_Panel* ui_panel_controller;

GLUI_Spinner* ui_spin_distance;
GLUI_Spinner* ui_spin_slices;
GLUI_Spinner* ui_spin_stacks;

GLUI_Checkbox* ui_check_wire;

GLUI_Rotation* ui_rot_rotation;

// GLUIのLive variable
// 回転マトリクス
float rot[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

// 視点の距離
float distance = 5.0;

// planetで球生成にもちいる変数
int slices = CPlanetCustom::SLICES;
int stacks = CPlanetCustom::STACKS;

// ワイヤーフレームで表示するかしないか
int is_wire = 0;

// GLUIコールバック用の識別子
enum {
  ID_SPINNER_SLICES,
  ID_SPINNER_STACKS,
  ID_CHECKBOX_WIRE
};

// デフォルトライト
static const GLfloat light_position[] = { -1.0, 1.0, 1.0, 0.0 };
static const GLfloat light_ambient[] = {0.1, 0.1, 0.1, 1.0};
static const GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};

// デフォルトマテリアル
static const GLfloat mat_default_color[] = { 1.0, 1.0, 1.0, 1.0 };
static const GLfloat mat_default_specular[] = { 0.0, 0.0, 0.0, 0.0 };
static const GLfloat mat_default_shininess[] = { 100.0 };
static const GLfloat mat_default_emission[] = {0.0, 0.0, 0.0, 0.0};

// 惑星
CPlanetCustom planet;

//-----------------------------------------------------------------------------------
// GLUIのコールバック関数
//-----------------------------------------------------------------------------------
void control_cb( int control )
{
  switch( control ) {
  case ID_SPINNER_SLICES:
    
    // 課題
    // planetのslicesを変更する
    // planet.hpp及びplanet_custom.hppを参考に。    
    
    break;
    
  case ID_SPINNER_STACKS:
    
    // 課題
    // planetのstacksを変更する
    // planet.hpp及びplanet_custom.hppを参考に。    
    
    break;
  
  case ID_CHECKBOX_WIRE:
    
    // 課題
    // planetがWIREFRAME表示とSOLID表示を切替える
    // planet.hpp及びplanet_custom.hppを参考に。    
    break;
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
  
  ui_spin_slices = glui->add_spinner_to_panel( ui_panel_controller, "Slices", GLUI_SPINNER_INT, /*課題*/, ID_SPINNER_SLICES, /*課題*/ );
  ui_spin_slices->set_int_limits( 3, 150 );
  ui_spin_slices->set_speed( 0.03 );
  
  ui_spin_stacks = glui->add_spinner_to_panel( ui_panel_controller, "Stacks", GLUI_SPINNER_INT, /*課題*/, ID_SPINNER_STACKS, /*課題*/ );
  ui_spin_stacks->set_int_limits( 3, 150 );
  ui_spin_stacks->set_speed( 0.03 );
  
  ui_check_wire = glui->add_checkbox_to_panel( ui_panel_controller, "Wireframe", /*課題*/, ID_CHECKBOX_WIRE, /*課題*/ );
  ui_check_wire->set_int_val( is_wire );
  
  glui->add_separator_to_panel( ui_panel_controller );
  
  ui_spin_distance = glui->add_spinner_to_panel( ui_panel_controller, "Distance", GLUI_SPINNER_FLOAT, /*課題*/ );
  ui_spin_distance->set_float_limits( 1.5, 10.0 );
  ui_spin_distance->set_speed( 0.1 );
  
  ui_rot_rotation = glui->add_rotation_to_panel( ui_panel_controller, "Rotation", /*課題*/ );
  
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
  // CPlanetCustomクラスの初期化を行う
  // planet.hpp及びplanet_custom.hppを参考に。
  
}

//-----------------------------------------------------------------------------------
// レンダリング
//-----------------------------------------------------------------------------------
void display(void)
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  // 視点の設定
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef( 0.0, 0.0, -distance );
  glMultMatrixf( rot );
  
  // 課題
  // CPlanetCustomクラスにより惑星の描画を行う。
  // planet.hpp及びplanet_custom.hppを参考に。
  
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
  gluPerspective(60.0, (float)tw / (float)th, 0.5, 20.0);
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
  glutPostRedisplay();
}

//-----------------------------------------------------------------------------------
// マウス移動のコールバック関数
//-----------------------------------------------------------------------------------
void motion(int x, int y)
{
  glutPostRedisplay();
}

//-----------------------------------------------------------------------------------
// アイドル時のコールバック関数
//-----------------------------------------------------------------------------------
void idle()
{
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
  glutInitWindowSize (640, 480); 
  glutInitWindowPosition (50, 50);
  main_window = glutCreateWindow ("Planet View");
  
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
