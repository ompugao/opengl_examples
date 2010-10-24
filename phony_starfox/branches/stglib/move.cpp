#include "object.h"
#include "key.h"
#include "array.h"
#include <GL/glut.h>
#include <iostream>
#include <cmath>

extern vector<Object>::iterator pv_object;

using namespace std;

// 自機専用
// vが速度
void Object::move_self()
{
    static bool old_key_b = false;
    // キー入力により移動
    // 上下左右
    if(key_left) {
        glRotated(5.0, 0,0,1);
    }
    if(key_right) {
        glRotated(-5.0, 0,0,1);
    }
    if(key_up) {
        glRotated(-5.0, 0,1,0);
    }
    if(key_down) {
        glRotated(5.0, 0,1,0);
    }
    // 前進 // 'c'でブースト
    if(key_c) {
        glTranslated(v*2,0,0);
    } else {
        glTranslated(v,0,0);
    }
    // 回転
    if(key_x) {
        glRotated(10, 1,0,0);
    }
    if(key_z) {
        glRotated(-10, 1,0,0);
    }
    // 弾を撃つ // 現在のcoordを渡す
    if(key_v) {
       add_object(B_STRAIGHT_S, BULLETA, 1, 0x00ff00, 50, 100, coord);
    }
    // レーザー // 現在のコードを渡す
    if(key_b && !old_key_b) {
        for(int i = 0; i<= 100; i++) {
            add_object(B_LASER_S, SPIRAL, 1, 0xffffaa, 10, i*10, coord);
        }
    }
    old_key_b = key_b;
}

// 円軌道
// vが角速度
// (detail_array[0][0],detail_array[0][1],detail_array[0][2])が中心座標
// (detail_array[1][0],detail_array[1][1],detail_array[1][2])が回転平面の法線ベクトル
// detail_array[2][0]が半径,detail_array[2][1]には初期角度を渡し内部で角度を保存するのに使用,detail_array[2][2]は使われない
void Object::move_circle()
{
    // 毎周期vだけ角度が増加
    // add_objectの引数で初期化されていない場合は0で初期化
    if(detail_array[2][1] == NULL_NUM) {
        detail_array[2][1] = 0;    
    }
    detail_array[2][1] += v;
    // 中心座標へ平行移動 // add_objectの引数で指定されているかいないかで場合分け
    if(detail_array[0][0] != NULL_NUM && detail_array[0][1] != NULL_NUM && detail_array[0][2] != NULL_NUM) {
        glTranslated(detail_array[0][0],detail_array[0][1],detail_array[0][2]);
    } else {
        glTranslated(0,0,0);
    }
    // 回転移動 // add_objectの引数で指定されているかいないかで場合分け
    if(detail_array[1][0] != NULL_NUM && detail_array[1][1] != NULL_NUM && detail_array[1][2] != NULL_NUM) {
        glRotated(detail_array[2][1], detail_array[1][0],detail_array[1][1],detail_array[1][2]);
    } else {
        glRotated(detail_array[2][1], 0,0,1);
    }
    // 半径分だけ平行移動 // add_objectの引数で指定されているかいないかで場合分け
    if(detail_array[2][0] != NULL_NUM) {
        glTranslated(detail_array[2][0],0,0);
    } else {
        glTranslated(100,0,0);
    }
}

// 直進
void Object::move_straight()
{
    glTranslated(v,0,0);
}

// リサージュ軌道
// vが角速度
// (detail_array[0][0],detail_array[0][1],detail_array[0][2])が中心座標
// detail_array[1][0]はxの角度の係数,detail_array[1][1]はyの角度の係数,detail_array[1][2]は位相差
// detail_array[2][0]がx方向半径,detail_array[2][1]はy方向半径,detail_array[2][2]には初期角度を渡し内部で角度を保存するのに使用
void Object::move_liss()
{
    // 毎周期vだけ角度が増加
    // add_objectの引数で初期化されていない場合は0で初期化
    if(detail_array[2][2] == NULL_NUM) {
        detail_array[2][2] = 0;    
    }
    detail_array[2][2] += v;
    // 座標計算
    double _x = detail_array[2][0] * cos(detail_array[1][0] * detail_array[2][2]) + detail_array[0][0];
    double _y = detail_array[2][1] * sin(detail_array[1][1] * detail_array[2][2] + detail_array[1][2]) + detail_array[0][1];
    double _z = detail_array[0][2];
    // 向き // ToDo:うまくいかない
//    glRotated(atan2(detail_array[1][1]*detail_array[1][2]*cos(detail_array[1][1]*detail_array[2][2]+detail_array[1][2]),
//    -detail_array[1][0]*detail_array[2][0]*sin(detail_array[1][0]*detail_array[2][2])),0,0,1);
    // 平行移動
    glTranslated(_x,_y,_z);
    // 回転移動 // add_objectの引数で指定されているかいないかで場合分け
    glRotated(detail_array[2][2], 0,0,1);
}

// 追従
// vが速度
void Object::move_chase()
{
    // 自機(SELF)までの相対位置
    double *relative_pos = new double [3];
    relative_pos = get_self_pos(coord);
    // 左右 // RAD2DEG(=180/3.14)はarray.cppで定義
    glRotated(atan2(relative_pos[1],relative_pos[0]) * RAD2DEG, 0,0,1);
    // 上下 // RAD2DEG(=180/3.14)はarray.cppで定義 // 左手系なので(-1)倍が必要
    glRotated(-atan2(relative_pos[2],relative_pos[0]) * RAD2DEG, 0,1,0);
    // 前進
    glTranslated(v,0,0);
    // 回転 // 自機に近づくと回転する
    if(abs(relative_pos[0]) < 500) {
        glRotated(10, 1,0,0);
    }
    // 弾を撃つ // 現在のcoordを渡す
    if(get_object_time_count()%10 == 0) {
        add_object(B_STRAIGHT, CUBE, 1, 0x00ff00, 50, 100, coord);
    }
    
    delete relative_pos;
}

// 静止
void Object::move_still()
{
    // 何もしない
}

// 爆発
void Object::move_explosion()
{
    R += 20;
    if(get_object_time_count() > 20) {
        hp = 0;
    }
}

// レーザー
// 最初にv+100だけx方向に平行移動した後静止、2周期で消える
void Object::move_laser()
{
    if(get_object_time_count() == 0) {
        // Awingの先頭
        glTranslated(v+100,0,0);
        glRotated(v,1,0,0);
    }
    if(get_object_time_count() > 2) {
        hp = 0;
    }target_object = pv_object;

}

// 自機前面 // カーソル用
void Object::move_front()
{
    glMultMatrixd(self_coord);
    glTranslated(500,0,0);
}
