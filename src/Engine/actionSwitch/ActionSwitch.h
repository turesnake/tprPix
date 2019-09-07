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

//-------------------- CPP --------------------//
#include <functional>
#include <memory>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "ActionSwitchType.h"
#include "BoolBitMap.h"


//--- need ---//
class GameObj;


class ActionSwitch{
public:
    using F_ACTION_SWITCH = std::function<void( GameObj&, ActionSwitchType)>;

    ActionSwitch( GameObj &goRef_ ):
        goRef(goRef_)
        {
            bitMap.init(bitMapBytes);
        }

    inline void bind_func( const F_ACTION_SWITCH &func_ )noexcept{
        func = func_;
    }

    void call_func( ActionSwitchType type_ );

    inline void clear_bitMap()noexcept{
        bitMap.clear_all();
    }

    //-- 登记某个 actionSwitch --
    inline void signUp( ActionSwitchType type_ )noexcept{
        bitMap.signUp( static_cast<u32_t>(type_) );
    }

    //-- 检查某个 actionSwitch 是否已登记 --
    inline bool check( ActionSwitchType type_ )noexcept{
        return  bitMap.check( static_cast<u32_t>(type_) );
    }
    

private:
    GameObj   &goRef;

    BoolBitMap  bitMap  {}; //- 位图，记录了本实例 注册了哪几个类型的 actionSwitch
                            //- 暂定上限为 64-bit
    //--- functor ----//
    F_ACTION_SWITCH  func {nullptr};

    //======== static ========//
    static size_t bitMapBytes; //- bitMap容器 占用多少 字节
                    //- 随着 ActionSwitchType 种类的增多，
                    //  这个值将不断扩大  
};

//-------- static --------//
//-- 在游戏运行期间，此值不会被改变
inline size_t ActionSwitch::bitMapBytes {8};


#endif 

