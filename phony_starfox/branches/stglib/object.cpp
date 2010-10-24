#include <GL/glut.h>
#include "object.h"
#include "array.h"
#include "time_counter.h"
#include <stdio.h>
#include <string.h>

// Objectのインスタンス(弾は除く)を格納するコンテナ // できるだけextern宣言しない方針で
vector<Object> v_object;
// イテレータ // collide()内でフィールド外に出たオブジェクトをerase()するためにグローバルでないといけない
vector<Object>::iterator pv_object;
// Objectのインスタンス(弾のみ)を格納するコンテナ // できるだけextern宣言しない方針で
vector<Object> v_bullet;
// イテレータ // collide()内でフィールド外に出たオブジェクトをerase()するためにグローバルでないといけない
vector<Object>::iterator pv_bullet;

// 自機の位置(座標変換行列) // 全インスタンス共通なのでstaticメンバ変数にする // staticメンバ変数の初期化はコンストラクタで行わない
double *Object::self_coord = new double [16];

// ロックオンしている敵を指すイテレータ // 全インスタンス共通なのでstaticメンバ変数にする // 先頭のインスタンス(地面)で初期化
vector<Object>::iterator Object::target_object;

// フィールドサイズ
double Object::FieldSize = 1000;

// コンストラクタ
Object::Object()
{
    // coordのメモリ領域確保
    coord = new GLdouble [16];
    loadIdentity(coord);
    // オブジェクトが生成された時点のタイムカウントをセット
    init_time_count = time_count;
}

// コピーコンストラクタ // メンバ変数が増えたら追記する
Object::Object(const Object &_object)
{
    // メモリ領域を新しく確保
    coord = new GLdouble [16];
    // 行列の中身をコピー
    copyMatrix(_object.coord, coord);
    // 他のメンバ変数はそのまま代入する
    R = _object.R;
    hp = _object.hp;
    move_type = _object.move_type;
    detail_type = _object.detail_type;
    // ToDo:後にcolorがラベル化されるので書き変えられる
    for(int i = 0; i <= 4; i++) {
        color[i] = _object.color[i];
    }
    v = _object.v;
    for(int i = 0; i <= 2; i++) {
        for(int j = 0; j <= 2; j++) {
            detail_array[i][j] = _object.detail_array[i][j];
        }
    }
    init_time_count = _object.init_time_count;
}

// デストラクタ
Object::~Object(){
    // フィールド外に出た弾をerase()する際にdeleteするとSEGVになる // でもdeleteしないとメモリリークしてる気がする
    // delete [] coord;
}

// オブジェクトの中心位置を返す // 返り値はdouble型3要素の配列の先頭要素のアドレス // 当たり判定で使用
double *Object::get_pos()
{
    double *_pos;
    _pos = new double[3];
    for(int i = 0; i <= 2; i++) {
        _pos[i] = coord[12 + i];
    }
    return _pos; 
}

// 自機の中心位置を返す // 返り値はdouble型3要素の配列の先頭要素のアドレス // 引数により座標系で測るかを指定できる、デフォルトはワールド座標系 // 自機追従で使用
double *Object::get_self_pos(double *origin_coord)
{
    // origin_coordの逆行列
    double *inv_coord = new double [16];
    // 掛け算後の行列
    double *tmp_coord = new double [16];
    // origin_coordが指定されていたら、origin_coordの逆行列(inv_coord)とself_coordを掛け結果をtmp_coordに格納
    if(origin_coord != NULL) {
        inverse(origin_coord, inv_coord);
        multiply(self_coord, inv_coord, tmp_coord);
    }
    // 返り値のポインタ
    double *_pos = new double [3];
    // pos[i]に値を代入
    for(int i = 0; i <= 2; i++) {
        // origin_coordが指定されているかいないかで場合分け
        if(origin_coord != NULL) {
            _pos[i] = tmp_coord[12 + i];
        } else {
            _pos[i] = self_coord[12 + i];
        }
    }
    // メモリ開放
    delete inv_coord;
    delete tmp_coord;
    return _pos; 
}

// 当たり判定半径を返す // 当たり判定で使用
double Object::get_R()
{
    return R;
}

void Object::pre_move()
{
    glLoadIdentity();
    // 以下LocalとWorldで場合分け
    if(move_type >= 100) {
        // LocalObject
         // 前周期のローカル座標系に移動
         if (coord != NULL) {
            glMultMatrixd(coord);
        }
    } else {
        // WorldObject
    }
}

// フィールド外に出た弾などのインスタンス消去もここで行う
// 返り値はインスタンスが消去されない場合0,消去される場合1 // ここではhpを0にするだけで実際のeraseはremove()内で行う
int Object::post_move()
{
    // 弾がフィールド外に出たら消す
    if(move_type >= 200) {
    // 弾の場合 // 弾のラベルは200以上
        if(abs((get_pos())[0]) > FieldSize / 2 || 
           abs((get_pos())[1]) > FieldSize / 2 || 
           abs((get_pos())[2] - FieldSize / 4) > FieldSize / 4) {
            // フィールド外に出たらhpを0にする
            hp = 0;
            // オブジェクトを消去する場合は返り値1
            return 1;
        } else {
            // フィールド外に出てなければそのまま
        }
    } else {
    // 弾以外の場合 // とりあえずフィールド外に出ても何もしない
    }
    // 現在のローカル座標系を保存
    glGetDoublev(GL_MODELVIEW_MATRIX,coord);
    // 自機はcoordをself_coordにコピーする
    if(move_type == SELF) {
        copyMatrix(coord, self_coord);
    }
     //カメラの行列をかけ合わせ,設定
    multiply_camera_matrix(); 
    // 以下LocalとWorldで場合分け
    if(move_type >= 100) {
        // LocalObject
    } else {
        // WorldObject
    }
    // オブジェクトを消去しない場合は返り値0
    return 0;
}

// post_move()でのみ使用
void Object::multiply_camera_matrix(){
    GLdouble tmp[16];
    glLoadIdentity();
    multiply(coord, camera_matrix, tmp);
    glLoadMatrixd(tmp);
}

// 当たり判定を行う // draw()から呼ばれる
void Object::collide()
{
    // 自機当たり判定 // 自機のmove_typeはSELF // ToDo:味方機がいる場合について考える
    if(move_type == SELF) {
        // 敵機に対して当たり判定 // イテレータはグローバルのものと競合しないようにローカルにしないといけない
        for(vector<Object>::iterator tmp_pv_object = v_object.begin(); tmp_pv_object != v_object.end(); tmp_pv_object++) {
            // 敵機に対して当たり判定 // move_typeがSELF以外なら敵機
            if((*tmp_pv_object).move_type != SELF && (*tmp_pv_object).move_type != EXPLOSION && (*tmp_pv_object).detail_type != GROUND) {
                if(size_array(subst_array((*tmp_pv_object).get_pos(), get_pos())) < ((*tmp_pv_object).get_R() + get_R())) {
                    // 敵機に当たった場合の処理
//                    cout << "you collided with an enemy." << endl;
                    hp--;
                    (*tmp_pv_object).hp--;
                }
            }
        }
        // 自弾以外の弾に対して当たり判定 // イテレータはグローバルのものと競合しないようにローカルにしないといけない
        for(vector<Object>::iterator tmp_pv_bullet = v_bullet.begin(); tmp_pv_bullet != v_bullet.end(); tmp_pv_bullet++) {
            // 自弾以外の弾に対して当たり判定 // 敵弾のmove_typeは300未満
            if((*tmp_pv_bullet).move_type < 300) {
                if(size_array(subst_array((*tmp_pv_bullet).get_pos(), get_pos())) < ((*tmp_pv_bullet).get_R() + get_R())) {
                    // 敵弾に当たった場合の処理
//                    cout << "you collided with an enemy's bullet."<< endl;
                    hp--;
                    (*tmp_pv_bullet).hp--;
                }
            }
        }
    }
    // 自弾当たり判定 // move_type300以上が自弾
    if(move_type >= 300) {
        // イテレータはグローバルのものと競合しないようにローカルにしないといけない
        for(vector<Object>::iterator tmp_pv_object = v_object.begin(); tmp_pv_object != v_object.end(); tmp_pv_object++) {
            // 自機以外の機に対して当たり判定
            if((*tmp_pv_object).move_type != SELF && (*tmp_pv_object).move_type != EXPLOSION && (*tmp_pv_object).detail_type != GROUND && (*tmp_pv_object).detail_type != CURSOR) {
                if(size_array(subst_array((*tmp_pv_object).get_pos(), get_pos())) < ((*tmp_pv_object).get_R() + get_R())) {
                    // 敵機に当たった場合の処理
//                    cout << "you shotted an enemy."<< endl;
                    hp--;
                    (*tmp_pv_object).hp--;
                }
            }
            // 自機以外の機に対して当たり判定
            if((*tmp_pv_object).move_type != SELF && (*tmp_pv_object).move_type != EXPLOSION && (*tmp_pv_object).detail_type != GROUND && (*tmp_pv_object).detail_type != CURSOR) {
                if(size_array(subst_array((*tmp_pv_object).get_pos(), get_pos())) < ((*tmp_pv_object).get_R() + get_R())) {
                    // 敵機に当たった場合の処理
//                    cout << "you shotted an enemy."<< endl;
                    hp--;
                    (*tmp_pv_object).hp--;
                }
            }
        }
    }
}

// オブジェクトを消去する
void Object::remove()
{
    if(hp <= 0) {
        // グローバルなイテレータをいじる // 返り値をイテレータに代入する
        if(move_type >= 200) {
            // 弾の場合 // 弾のラベルは200以上
            pv_bullet = v_bullet.erase(pv_bullet);
        } else {
            // 弾以外の場合
            if (move_type != EXPLOSION) {
             // ToDo:登録するオブジェクト数が少ないとここでSEGVになる // 理由がわからん
             add_object(EXPLOSION, SPHERE, 100, 0xff0c0c, 10, 500, coord); // tmp // 爆発してるつもり
            }
            pv_object = v_object.erase(pv_object);
        }
    } else {
        // 消去されないときはグローバルなイテレータをインクリメント
        if(move_type >= 200) {
            pv_bullet++;
        } else {
            pv_object++;
        }
    }
}

// オブジェクトが生成されてから現在までのカウント数を返す
int Object::get_object_time_count()
{
    return (time_count - init_time_count);
}

// MOVE_TYPEを追加するたびに内部の場合分けを追加すること
void Object::move()
{
    switch(move_type) {
        case SELF:
            move_self();
            break;
        case CIRCLE:
            move_circle();
            break;
         // 以下の3つのラベルは同じ動きをする // ラベルの区別は当たり判定で利用される
        case B_STRAIGHT_S:
        case STRAIGHT:
        case B_STRAIGHT:
            move_straight();
            break;
        case LISS:
            move_liss();
            break;
        case CHASE:
            move_chase();
            break;
        case STILL:
            move_still();
            break;
        case EXPLOSION:
            move_explosion();
            break;
        case B_LASER_S:
        case B_LASER:
            move_laser();
            break;
        case FRONT:
            move_front();
            break;
    }
}

// DETAIL_TYPEを追加するたびに内部の場合分けを追加すること
void Object::detail()
{
  switch(detail_type) {
  case SPHERE:
    detail_sphere();
    break;
  case CUBE:
    detail_cube();
    break;
  case GROUND:
    detail_ground();
    break;
  case AWING:
    detail_Awing();
    break;
  case BULLETA:
    detail_bulletA();
    break;
  case SPIRAL:
    detail_spiral();
    break;
  case CURSOR:
    detail_cursor();
    break;
  }
}

// 毎周期呼ばれる
void Object::draw()
{
    // 移動、coord更新
    pre_move();
    move();
    // フィールド外に出た弾などのインスタンス消去はpost_move()内で行う // 消去されると返り値が1になる
    if(post_move() == 0) {
        // 描画
        detail();
        // 当たり判定
        collide();
    }
    // オブジェクト消去
    remove();
}

// メンバ変数に代入 // add_object()内でのみ使用
void Object::set_object(MOVE_TYPE _move_type, DETAIL_TYPE _detail_type, double _hp, double *_color, double _R, double _v, double *_coord, double *_detail_array1, double *_detail_array2, double *_detail_array3)
{
    move_type = _move_type;
    detail_type = _detail_type;
    for(int i = 0; i <= 3; i++) {
        color[i] = _color[i];
    }
    hp = _hp;
    R = _R;
    v = _v;
    if(_coord != NULL) {
        copyMatrix(_coord, coord);
    }
    // detail_arrayはNULLだったら全要素にNULL_NUM(array.cppで定義)を入れておく
    if(_detail_array1 != NULL) {
        for(int i = 0; i <= 2; i++) {
            detail_array[0][i] = _detail_array1[i];
        }
    } else {
        for(int i = 0; i <= 2; i++) {
            detail_array[0][i] = NULL_NUM;
        }
    }
    if(_detail_array2 != NULL) {
        for(int i = 0; i <= 2; i++) {
            detail_array[1][i] = _detail_array2[i];
        }
    } else {
        for(int i = 0; i <= 2; i++) {
            detail_array[1][i] = NULL_NUM;
        }
    }
    if(_detail_array3 != NULL) {
        for(int i = 0; i <= 2; i++) {
            detail_array[2][i] = _detail_array3[i];
        }
    } else {
        for(int i = 0; i <= 2; i++) {
            detail_array[1][i] = NULL_NUM;
        }
    }
}

// 敵を追加する // この関数を呼ぶだけで敵が追加され、毎周期draw()や当たり判定を実行してくれる // ToDo:引数の意味は後でmanualにまとめる
void add_object(MOVE_TYPE _move_type, DETAIL_TYPE _detail_type, double _hp, int _int_color, double _R, double _v, double *_coord, double *_detail_array1, double *_detail_array2, double *_detail_array3)
{
    // インスタンスを作る
    Object _object;
    double _color[4];
    // 16進数6ケタで表されているRGBをdouble型3要素の配列に合わせて変換する
    for(int i = 0; i <= 2; i++) {
        _color[2-i] = (double)((_int_color >> i * 8) & 0x0000ff) / 0x0000ff;
    }
    // 4つ目の要素は1.0にしておく
    _color[3] = 1.0;
    // set_objectを呼び、インスタンスのメンバ変数をセットする
    _object.set_object(_move_type, _detail_type, _hp, _color, _R, _v, _coord, _detail_array1, _detail_array2, _detail_array3);
    // インスタンスをコンテナにpush_backする // 弾と弾以外で、push_backされるコンテナが違う
    if(_move_type >= 200) {
        // 弾はv_bulletにpush_back // 弾のmove_typeは200以上
        v_bullet.push_back(_object);
    } else {
        // 弾以外はv_objectにpush_bsck
        v_object.push_back(_object);
    }
}

// v_object,v_bullet内の全てのインスタンスのdrawを実行する
void draw_object()
{
    for(pv_object = v_object.begin(); pv_object != v_object.end();/* pv_object++*/) { // インクリメントはremove()内で
        (*pv_object).draw();
    }
    for(pv_bullet = v_bullet.begin(); pv_bullet != v_bullet.end();/* pv_bullet++*/) { // インクリメントはremove()内で
        (*pv_bullet).draw();
    }
}
