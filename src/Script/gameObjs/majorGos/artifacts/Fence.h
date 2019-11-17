/*
 * ========================= Fence.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_FENCE_H
#define TPR_GO_FENCE_H

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "DyParam.h"

namespace gameObjs{//------------- namespace gameObjs ----------------

class Fence{
public:
    static void init(GameObj &goRef_, const DyParam &dyParams_ );
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

