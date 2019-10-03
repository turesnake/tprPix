/*
 * ========================= ActionSwitch.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.28
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "ActionSwitch.h"


//-------------------- Engine --------------------//
#include "GameObj.h"


//-------- static --------//
//-- 在游戏运行期间，此值不会被改变
size_t ActionSwitch::bitMapBytes = 8;


void ActionSwitch::call_func( ActionSwitchType type_ ){
    if( this->func == nullptr ){
        return;
    }
    //-- 如果本实例 没装载此 _type，也将安全返回 --
    if( check(type_) == false ){
        return;
    }
    //-- 正式call --
    this->func( this->goRef, type_ );
                //-- 临时系的写法，不合理 ...
                //...
}




