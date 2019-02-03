/*
 * ========================= Fly.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.12
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   飞行模式，不受 CrossState 影响。
 * 
 * ----------------------------
 */
#ifndef _TPR_FLY_H_
#define _TPR_FLY_H_

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec2
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4
#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptr

//-------------------- Engine --------------------//
#include "FlyIns.h"


//-- 依赖代码 --
class Move;
class GameObjPos;


class Fly{
public:
    Fly() = default;

    inline void init( Move *_movePtr, GameObjPos *_goPosPtr ){
        movePtr  = _movePtr;
        goPosPtr = _goPosPtr;
    }

    void RenderUpdate(); 


    inline void set_newflyIns( const FlyIns &_newFlyIns ){
        newflyIns = _newFlyIns;
    }

    inline void set_maskCountLimit( int _limit ){
        maskCountLimit = _limit;
    }

    //-- 新功能：fly模式时，让自己对齐到最近的 可使用 mapent组。
    //-- 这个功能有一些潜在问题：
    //-- 如果 附近没有可用的 mapent集 怎么办，（比如飞到一片树林头顶）
    //   一种设计是 无视 mapent占有问题，让 飞行go 直挺挺掉下去，将下方的 go砸出点伤害
    //
    //
    //...

private:
    void calc_dir();

    //======== vals ========//
    Move        *movePtr  {nullptr}; //- 每个 fly实例 都属于一个 move实例, 强关联
    GameObjPos  *goPosPtr {nullptr};                           

    FlyIns    newflyIns     { 0.0f };
    FlyIns    currentFlyIns { 0.0f };

    //-- 暂时未使用 --
    int  maskCountLimit {1}; //- 屏蔽计数器上限
    int  maskCount      {0}; //- val

    glm::vec2  translate {}; //- tmp,本帧位移向量

};

#endif 

