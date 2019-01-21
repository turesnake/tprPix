/*
 * ========================= Collision.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.08
 *                                        修改 -- 2019.01.08
 * ----------------------------------------------------------
 *   碰撞模块
 * ----------------------------
 */
#ifndef _TPR_COLLISION_H_
#define _TPR_COLLISION_H_

//-------------------- CPP --------------------//
//#include <vector>
#include <string>

//-------------------- Engine --------------------//
#include "NineBox.h"


//--- need ---//
class GameObj;
class GameMesh;


//-- 和 Mesh 一样，也是一个 “整合类”
//
class Collision{
public:
    Collision() = default;

    void init( GameObj *_goPtr );

    void collide_for_crawl( const NineBoxIdx &_nbIdx ); //-- 碰撞检测主流程 
                    //-- 临时的，局限性很大...


 
private:
    GameObj  *goPtr       {nullptr};


};









#endif 

