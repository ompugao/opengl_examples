#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "matrix.h"
#include "camera.h"
#include <vector>
using namespace std;

// 動きのラベル // 新しい動きを実装するときはここにラベルを追加 // 4つに分かれているので該当するところに追加する
typedef enum _MOVE_TYPE{
    // LocalObject
    SELF = 100, // 自機専用 // 1つのみ
    STRAIGHT, // 直進
    CHASE, // 自機追従
    STILL, // 静止
    EXPLOSION, // 爆発
    // WorldObject
    CIRCLE = 1, // 円
    LISS, // リサージュ
    FRONT, // 自機前方
    // Bullet(敵弾,Localに限る)
    B_STRAIGHT = 200, // 直進(敵弾)
    B_LASER, // レーザー(敵弾)
    // Bullet_Self(自弾,Localに限る)
    B_STRAIGHT_S = 300, // 直進(自弾)
    B_LASER_S // レーザー(自弾)
} MOVE_TYPE;

// 形状のラベル // 新しい形状を実装するときはここにラベルを追加
typedef enum _DETAIL_TYPE{
    // 全ての形状
    SPHERE, // 球
    CUBE, // 立方体
    GROUND, // 平面 // 1つのみ
    AWING, // Awing型
    BULLETA, // 弾A型
    SPIRAL, // 螺旋
    CURSOR // カーソル
} DETAIL_TYPE;

class Object{
    public:
        // 自機の位置(座標変換行列) // 全インスタンス共通なのでstaticメンバ変数にする
        static double *self_coord;
    protected:
        // 位置(座標変換行列)
        double *coord;
        // 当たり判定半径
        double R;
        // ロックオンしている敵を指すイテレータ // 全インスタンス共通なのでstaticメンバ変数にする
        static vector<Object>::iterator target_object;
        // フィールドサイズ // 全インスタンス共通なのでstaticメンバ変数にする
        static double FieldSize;
        // HP
        int hp;
        // 動きのラベル
        MOVE_TYPE move_type;
        // 形状のラベル
        DETAIL_TYPE detail_type;
        // 色
        double color[4]; // ToDo:後にラベルに変わる予定 // 変えたらobject.cpp内のコピーコンストラクタを書き変えること
        // 速度
        double v;
        // オブジェクトが生成された時点のタイムカウント
        int init_time_count;
        // 具体的な軌道や形状を指定する変数 // move_type,detail_typeによって違った意味を持つ // 別案募集
        double detail_array[3][3];
        // 動き
        void pre_move();
        void move();
        int post_move();
        // 形状
        void detail();
        // 具体的な動き // MOVE_TYPEを追加するたびに以下へ関数を追加すること
        void move_self();
        void move_circle();
        void move_straight();
        void move_liss();
        void move_chase();
        void move_still();
        void move_explosion();
        void move_laser();
        void move_front();
        // 具体的な形状 // DETAIL_TYPEを追加するたびに以下へ関数を追加すること
        void detail_sphere();
        void detail_cube();
        void detail_ground();
        void detail_Awing();
        void detail_bulletA();
        void detail_spiral();
        void detail_cursor();
        // 当たり判定を行う // draw()で呼ばれる
        void collide();
        // オブジェクトを消去する // draw()で呼ばれる
        void remove();
        // post_move()でのみ使用
        void multiply_camera_matrix();
        // オブジェクトが生成されてから現在までのカウント数を返す
        int get_object_time_count();
        // HP表示のため
        void showHP(int, int);
    public:
        // コンストラクタ
        Object();
        // コピーコンストラクタ
        Object(const Object &_object);
        // デストラクタ
        ~Object();
        // 毎周期呼び出す // 内部でpre_move(),move(),post_move(),detail(),collide(),remove()をこの順番で呼び出す
        void draw();
        // オブジェクトの中心位置を返す // 返り値はdouble型3要素の配列の先頭要素のアドレス // 当たり判定で使用
        double *get_pos();
        // 自機の中心位置を返す // 返り値はdouble型3要素の配列の先頭要素のアドレス // 引数により座標系で測るかを指定できる、デフォルトはワールド座標系 // 自機追従で使用
        static double *get_self_pos(double *origin_coord = NULL);
        // 当たり判定半径を返す // 当たり判定で使用
        double get_R();
        // メンバ変数に代入 // add_object()内でのみ使用
        void set_object(MOVE_TYPE _move_type, DETAIL_TYPE _detail_type, double _hp, double *_color, double _R, double _v, double *_coord, double *_detail_array1, double *_detail_array2, double *_detail_array3);
};

// 敵を追加する // この関数を呼ぶだけで敵が追加され、毎周期draw()や当たり判定を実行してくれる // ToDo:引数の意味は後でmanualにまとめる
void add_object(MOVE_TYPE _move_type, DETAIL_TYPE _detail_type, double _hp, int _int_color, double _R, double _v, double *_coord = NULL, double *_detail_array1 = NULL, double *_detail_array2 = NULL, double *_detail_array3 = NULL);

// v_object,v_bullet内の全てのインスタンスのdrawを実行する
void draw_object();

// Objectのインスタンス(弾は除く)を格納するコンテナ // できるだけextern宣言しない方針で
//extern vector<Object> v_object;
// Objectのインスタンス(弾のみ)を格納するコンテナ // できるだけextern宣言しない方針で
//extern vector<Object> v_bullet;

#endif //__OBJECT_H__
