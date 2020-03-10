/*
 * ========================= ActionFSM.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   动作状态机
 *  -------------
 *  一个 go实例拥有一个 actionFSM实例，
 *  统管 所有 gomesh实例的 动画播放，
 *  控制 move 指令输入，（进一步控制 collision ）
 *  
 * ----------------------------
 */
#ifndef TPR_ACTION_FSM_H
#define TPR_ACTION_FSM_H

#include "pch.h"

//-------------------- Engine --------------------//
#include "functorTypes.h"


//- 节点状态 --
class ActionState{
public:

    F_void  enterFunc {nullptr}; //- 入口函数 
    F_void  exitFunc  {nullptr}; //- 出口函数

};



//-- need --//
class GameObj;


class ActionFSM{
public:
    ActionFSM() = default;

    inline void init( GameObj *goPtr_ )noexcept{
        this->goPtr = goPtr_;
    }


    inline ActionState *insert_new_state( const std::string &name_ )noexcept{
        // ***| INSERT FIRST, INIT LATER  |***
        ActionState state {};
        auto [insertIt, insertBool] = states.emplace( name_, state ); //- copy
        tprAssert( insertBool );

        // init...
        
        return &(insertIt->second);
    }


private:
    GameObj *goPtr {nullptr};

    std::unordered_map<std::string, ActionState> states {};
                    //- 暂用 string 来索引，效率可能有点低...

};



#endif

