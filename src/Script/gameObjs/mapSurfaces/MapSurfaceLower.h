/*
 * ===================== MapSurfaceLower.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_MAP_SURFACE_LOWER_H
#define TPR_GO_MAP_SURFACE_LOWER_H

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "DyParam.h"

namespace gameObjs{//------------- namespace gameObjs ----------------

class MapSurfaceLower{
public:
    static void init(GameObj &goRef_, const DyParam &dyParams_ );
private:                            
    //--- callback ---//
    static void OnRenderUpdate( GameObj &goRef_ ); 
    static void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );
};


}//------------- namespace gameObjs: end ----------------
#endif 


