/*
 * ========================= Bush.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.10.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_BUSH_C_H
#define TPR_GO_BUSH_C_H

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "DyParam.h"

namespace gameObjs{//------------- namespace gameObjs ----------------


// 类似 Grass，Bush 也是数种 bush亚种 的合并类。
// 它们拥有相同的 动画习性（风吹）但在被具体破坏后，会调用不同的 技能函数（未实现）
// 如果某种 bush 比较特殊，未来就要为它制作专门的 具象go类
class Bush{
public:
    static void init(GameObj &goRef_,const DyParam &dyParams_ );
private:                            
    static void bind( GameObj &goRef_ );
    static void rebind( GameObj &goRef_ );

    //--- callback ---//
    static void OnRenderUpdate( GameObj &goRef_ ); 
    static void OnLogicUpdate( GameObj &goRef_ ); 
    static void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );
};


}//------------- namespace gameObjs: end ----------------
#endif 

