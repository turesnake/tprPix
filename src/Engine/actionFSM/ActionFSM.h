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


//- 节点状态 --
class ActionState{
    



};



//-- need --//
class GameObj;


class ActionFSM{
public:
    ActionFSM() = default;

    inline void init( GameObj *_goPtr ){
        goPtr = _goPtr;
    }



private:
    GameObj *goPtr {nullptr};


};



#endif

