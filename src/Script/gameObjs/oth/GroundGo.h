/*
 * ===================== GroundGo.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.28
 *                                        MODIFY --
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_GROUND_GO_H
#define TPR_GO_GROUND_GO_H

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
#include "DyParam.h"
#include "animSubspeciesId.h"


namespace gameObjs{//------------- namespace gameObjs ----------------


class GroundGo{
public:
    //--- 延迟init ---//
    static void init(GameObj &goRef_, const DyParam &dyParams_ );
private:                            
    //--- callback ---//
    static void OnRenderUpdate( GameObj &goRef_ ); 
    static void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );
};


}//------------- namespace gameObjs: end ----------------
#endif 


