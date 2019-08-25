/*
 * ===================== MapSurfaceLower.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_MAP_SURFACE_LOWER_H
#define TPR_MAP_SURFACE_LOWER_H

//-------------------- CPP --------------------//
#include <string>
#include <vector>
#include <functional>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "GameObj.h"
#include "GameObjMesh.h"
#include "AnimFrameSet.h"
#include "MapAltitude.h"
#include "Density.h"
#include "ParamBinary.h"


namespace gameObjs{//------------- namespace gameObjs ----------------


struct MapSurfaceLower_PvtBinary{
    //size_t   lichen_ForestId {0};
            //- 简单的从 几种款式中，随机挑选一款 [0,7]
    int tmp {};
    //===== padding =====//
    //...
};


class MapSurfaceLower{
public:
    MapSurfaceLower() = default;

    //--- 延迟init ---//
    static void init_in_autoMod(GameObj &goRef_,
                            const ParamBinary &dyParams_ );

private:

    //--  每次调用回调函数，都需要做的 指针重绑定 --
    inline static  MapSurfaceLower_PvtBinary *rebind_ptr( GameObj &goRef_ ){
        return reinterpret_cast<MapSurfaceLower_PvtBinary*>(goRef_.get_pvtBinaryPtr());
    }
                            
    //--- callback ---//
    static void OnRenderUpdate( GameObj &goRef_ ); 
    static void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );
};


}//------------- namespace gameObjs: end ----------------
#endif 


