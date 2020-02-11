/*
 * ======================== Trough.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_TROUGH_H
#define TPR_GO_TROUGH_H

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "DyParam.h"

namespace gameObjs{//------------- namespace gameObjs ----------------

class Trough{
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

