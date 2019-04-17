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
#ifndef _TPR_ACTION_FSM_H_
#define _TPR_ACTION_FSM_H_


//-------------------- CPP --------------------//
#include <functional>
#include <unordered_map>
#include <string>


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

    inline void init( GameObj *_goPtr ){
        goPtr = _goPtr;
    }


    inline ActionState *insert_new_state( const std::string &_name ){
        // ***| INSERT FIRST, INIT LATER  |***
        ActionState state {};
        states.insert({ _name, state }); //- copy
        // init...
        return static_cast<ActionState*>( &(states.at(_name)) );
    }


private:
    GameObj *goPtr {nullptr};

    std::unordered_map<std::string, ActionState> states {};
                    //- 暂用 string 来索引，效率可能有点低...

};



#endif

