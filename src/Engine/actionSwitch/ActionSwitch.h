/*
 * ========================= ActionSwitch.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.02.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    一个 go实例 在不同的状态，需要切换到不同的 action
 *    一部分 切换工作 是在 引擎内部调用的。
 *    通过本模块来 统一管理
 * ----------------------------
 */
#ifndef _TPR_ACTION_SWITCH_H_
#define _TPR_ACTION_SWITCH_H_

//-------------------- CPP --------------------//
#include <functional>

//------------------- Libs --------------------//
#include "tprDataType.h" 

//-------------------- Engine --------------------//
#include "ActionSwitchType.h"
#include "BoolBitMap.h"


//--- need ---//
class GameObj;


class ActionSwitch{
public:
    using F_ACTION_SWITCH = std::function<void(GameObj*, ActionSwitchType)>;

    ActionSwitch() = default;

    inline void init( GameObj *_goPtr ){
        goPtr = _goPtr;
        bitMap.init(bitMapBytes);
    }

    inline void bind_func( const F_ACTION_SWITCH &_func ){
        func = _func;
    }

    inline void call_func( ActionSwitchType _type ){
        if( func == nullptr ){
            return;
        }
        //-- 如果本实例 没装载此 _type，也将安全返回 --
        if( check(_type) == false ){
            return;
        }
        //-- 正式call --
        func( goPtr, _type );
    }

    inline void clear_bitMap(){
        bitMap.clear_all();
    }

    //-- 登记某个 actionSwitch --
    inline void signUp( ActionSwitchType _type ){
        bitMap.signUp( (u32)_type );
    }

    //-- 检查某个 actionSwitch 是否已登记 --
    inline bool check( ActionSwitchType _type ){
        return  bitMap.check( (u32)_type );
    }
    

private:
    GameObj *goPtr {nullptr};

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

