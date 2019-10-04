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


//-------------------- CPP --------------------//
#include <functional>
#include <unordered_map>
#include <string>


//-------------------- Engine --------------------//
#include "tprAssert.h"


//- 节点状态 --
class ActionState{
public:
    using F_TMP = std::function<void()>;

    F_TMP  enterFunc {nullptr}; //- 入口函数 
    F_TMP  exitFunc  {nullptr}; //- 出口函数

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
        auto outPair = states.insert({ name_, state }); //- copy
        tprAssert( outPair.second );

        // init...
        
        return &(outPair.first->second);
    }


private:
    GameObj *goPtr {nullptr};

    std::unordered_map<std::string, ActionState> states {};
                    //- 暂用 string 来索引，效率可能有点低...

};



#endif

