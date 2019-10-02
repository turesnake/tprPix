/*
 * ========================= OneEyeBoy.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   测试用，可以绑定任何 afs
 * 
 * ----------------------------
 */
#ifndef TPR_GO_ONE_EYE_BOY_H
#define TPR_GO_ONE_EYE_BOY_H

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <functional>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "GameObj.h"
#include "GameObjMesh.h"
#include "AnimFrameSet.h"
#include "Density.h"
#include "ParamBinary.h"
#include "animSubspeciesId.h"


namespace gameObjs{//------------- namespace gameObjs ----------------


class OneEyeBoy{
public:
    //--- 延迟init ---//
    static void init_in_autoMod(GameObj &goRef_, const ParamBinary &dyParams_ );
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

