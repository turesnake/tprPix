/*
 * ========================= WallA.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.06
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_WALL_A_H
#define TPR_GO_WALL_A_H

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



class WallA{
public:
    //--- 延迟init ---//
    static void init_in_autoMod(GameObj &goRef_,
                            const DyParam &dyParams_ );

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

