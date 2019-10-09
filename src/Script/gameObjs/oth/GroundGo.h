/*
 * ===================== GroundGo.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.28
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_GROUND_GO_H
#define TPR_GO_GROUND_GO_H

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "DyParam.h"

namespace gameObjs{//------------- namespace gameObjs ----------------


class GroundGo{
public:
    static void init(GameObj &goRef_, const DyParam &dyParams_ );
private:                            
    //--- callback ---//
    static void OnRenderUpdate( GameObj &goRef_ ); 
    static void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );
};


}//------------- namespace gameObjs: end ----------------
#endif 


