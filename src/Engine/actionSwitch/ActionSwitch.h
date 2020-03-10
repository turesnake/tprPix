/*
 * ========================= ActionSwitch.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    一个 go实例 在不同的状态，需要切换到不同的 action
 *    一部分 切换工作 是在 引擎内部调用的。
 *    通过本模块来 统一管理
 *  -----
 *   可能会被 ActionFSM 取代...
 * ----------------------------
 */
#ifndef TPR_ACTION_SWITCH_H
#define TPR_ACTION_SWITCH_H

#include "pch.h"

//-------------------- Engine --------------------//
#include "ActionSwitchType.h"
#include "BoolBitMap.h"


//--- need ---//
class GameObj;


class ActionSwitch{
public:
    using F_ACTION_SWITCH = std::function<void( GameObj&, ActionSwitchType)>;

    explicit ActionSwitch( GameObj &goRef_ ):
        goRef(goRef_)
        {
            bitMap.resize( 64,1 ); // bitMap容器 占了 64 个元素
                                    //- 随着 ActionSwitchType 种类的增多，
                                    //  这个值将不断扩大  
        }

    inline void bind_func( const F_ACTION_SWITCH &func_ )noexcept{ this->func = func_; }

    void call_func( ActionSwitchType type_ );

    inline void clear_bitMap()noexcept{ bitMap.clear_all(); }

    //-- 登记某个 actionSwitch --
    inline void signUp( ActionSwitchType type_ )noexcept{
        bitMap.signUp( static_cast<size_t>(type_) ); // can't use cast_2_siz_t
    }

    //-- 检查某个 actionSwitch 是否已登记 --
    inline bool check( ActionSwitchType type_ )noexcept{
        return  bitMap.check( static_cast<size_t>(type_) ); // can't use cast_2_siz_t
    }
    

private:
    GameObj   &goRef;

    BoolBitMap  bitMap  {}; //- 位图，记录了本实例 注册了哪几个类型的 actionSwitch
                            //- 暂定上限为 64-bit
    //--- functor ----//
    F_ACTION_SWITCH  func {nullptr}; 
};



#endif 

