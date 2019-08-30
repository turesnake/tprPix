/*
 * ========================= Collision.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_COLLISION_H
#define TPR_COLLISION_H

//-------------------- CPP --------------------//
#include <string>

//-------------------- Engine --------------------//
#include "NineBox.h"

//--- need ---//
class GameObj;


//-- 存在两种碰撞检测:
// --1-- 移动碰撞检测
// --2-- 技能碰撞检测
// 这两种 相互独立。并行存在
//   这意味着，go并不需要停下来才能 释放技能。 
//   当然，由于 动画制作的问题，大部分技能在施展期间，还是会强制停下 go 的移动
class Collision{
public:
    Collision( GameObj &goRef_ ):
        goRef(goRef_)
        {}

    bool detect_collision_for_move( const NineBoxIdx &nbIdx_ ); //-- 碰撞检测主流程 
                    //-- 临时的，局限性很大...

                    //  除了有 移动时触发的碰撞检测，还有另一帧 "技能碰撞检测"
                    //  在目前的设计中，2种碰撞检测是 相互独立的

    inline void set_isDoPass( bool val_ ){
        this->isDoPass = val_;
    }
    inline void set_isBePass( bool val_ ){
        this->isBePass = val_;
    }

    inline bool get_isDoPass() const {
        return this->isDoPass;
    }
    inline bool get_isBePass() const {
        return this->isBePass;
    }

private:
    bool isPass_Check( bool isBePass_ );

    //======== vals ========//
    GameObj    &goRef;

    //======== flags ========//
    //  下方这组 可能存在 逻辑漏洞，但先这么用着... 
    bool  isDoPass  {false};//- 当自己为 主动go 时，是否穿过 被动go；
                            //  如果本go 是 “穿透箭”。可将此值设置为 true
                            //  此时，本go 将无视 被动go 的状态，移动穿过一切 被动go
    bool  isBePass  {false};//- 当自己为 被动go 时，是否允许 主动go 穿过自己；
                            //  如果本go 是 “草”／“腐蚀液”，可将此值 设置为 true
                            //  此时，本go 将无法阻止任何 go 从自己身上 穿过
};


#endif 

