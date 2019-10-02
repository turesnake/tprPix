/*
 * ===================== PlayerGoCircle.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.19
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   
 * ----------------------------
 */
#ifndef TPR_GO_PLAYER_GO_CIRCLE_H
#define TPR_GO_PLAYER_GO_CIRCLE_H

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


class PlayerGoCircle{
public:
    //--- 延迟init ---//
    static void init_in_autoMod(GameObj &goRef_, const ParamBinary &dyParams_ );
private:
    //--- callback ---//
    static void OnRenderUpdate( GameObj &goRef_ ); 
    static void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );
};


}//------------- namespace gameObjs: end ----------------
#endif 

