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

//--- need ---//
class GameMesh;


//-- 和 Mesh 一样，也是一个 “整合类”
//
class Collision{
public:
    Collision() = default;

    void init( GameMesh *_gameMeshPtr );

    void refresh(); //-- 刷新所有数据 

    void collide(); //-- 碰撞检测主流程 


 
private:
    GameMesh *gameMeshPtr;

    //-- 检测是否要 refresh --
    std::string   currentaActionName    {};
    int           currentActionFrameIdx {};

    //----



};









#endif 

