#include "Awing.h"
#include "key.h"
#include "vector.h"
#include <GL/glut.h>
#include "time_counter.h"
#include "matrix.h"
#include "camera.h"

#include <iostream>
using namespace std;


Awing::Awing(){
  pos(250,250,50,0,0,0);
  cout << pos.x << " " << pos.y <<  " " <<  pos.z << endl;
  for(int i=0;i<=15;i++){
    if(i%5==0) coord[i]=1;
    else coord[i]=0;
  }
  R = 18;
  speed = 20;
}
Awing::~Awing(){
  delete [] coord;
}
void Awing::draw(){
  glPushMatrix();

  move();
  detail();
  
  glPopMatrix();
  
}

void Awing::move(){
  static bool initialize_flag = false;
  if(!initialize_flag){
    glTranslated(pos.x,pos.y,pos.z);
    //最初のローカル座標系を保存
    glGetDoublev(GL_MODELVIEW_MATRIX,coord);
    initialize_flag = true;
    cout << "matrix initialized!!" <<endl;

    //行列がどうなっているかの確認用標準出力

    cout << time_count <<endl;
    cout << coord[0] << " " << coord[4] << " "<< coord[8]  << " "<< coord[12] <<endl;
    cout << coord[1] << " " << coord[5] << " "<< coord[9]  << " "<< coord[13] <<endl;
    cout << coord[2] << " " << coord[6] << " "<< coord[10] << " "<< coord[14] <<endl;
    cout << coord[3] << " " << coord[7] << " "<< coord[11] << " "<< coord[15] <<endl;
    cout << endl;


  }
  
  glLoadMatrixd(coord);//前周期のローカル座標系に移動

  /**********************************
    以下、現周期で行う移動の処理
  **********************************/
  if(key_left){
    glRotated(5.0, 0,0,1);
  }
  if(key_right){
    glRotated(-5.0, 0,0,1);
  }
  if(key_up){
    glRotated(-5.0, 0,1,0);
  }
  if(key_down){
    glRotated(5.0, 0,1,0);
  }

  glTranslated(speed,0,0);
  if(key_z){
    glRotated(15, 1,0,0);
  }

  /**********************************
    ここまで
  **********************************/

  //現在のローカル座標系を保存
  glGetDoublev(GL_MODELVIEW_MATRIX,coord);
  
  //posの更新
  renew_pos();

  //行列がどうなっているかの確認用標準出力

  cout << time_count <<endl;
  cout << coord[0] << " " << coord[4] << " "<< coord[8]  << " "<< coord[12] <<endl;
  cout << coord[1] << " " << coord[5] << " "<< coord[9]  << " "<< coord[13] <<endl;
  cout << coord[2] << " " << coord[6] << " "<< coord[10] << " "<< coord[14] <<endl;
  cout << coord[3] << " " << coord[7] << " "<< coord[11] << " "<< coord[15] <<endl;
  cout << pos.x << " " << pos.y <<  " " <<  pos.z << endl;
  cout << endl;


}
void Awing::detail(){
  glColor3f(0.0,1.0,0.0);
  glPushMatrix();
  glRotated(90, 1,0,0);
  glutSolidTeapot(60.0);
  glPopMatrix();
}

void Awing::renew_pos(){
  double tmp[16];
  multiply(coord,inv_camera,tmp);
  pos.x = tmp[12];
  pos.y = tmp[13];
  pos.z = tmp[14];
}

VECTOR Awing::get_pos(){
  return pos;
}
double Awing::get_R(){
  return R;
}
