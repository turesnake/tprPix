/*
 * ========================= Wheat.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.11
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   小麦
 * ----------------------------
 */
#ifndef TPR_WHEAT_H
#define TPR_WHEAT_H

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


namespace gameObjs{//------------- namespace gameObjs ----------------

struct Wheat_PvtBinary{
    size_t   wheatId {0};
            //- 简单的从 几种款式中，随机挑选一款 [0,7]
    //===== padding =====//
    //...
};


class Wheat{
public:
    Wheat() = default;

    //--- 延迟init ---//
    static void init_in_autoMod(   goSpecId_t specID_,
                            GameObj &goRef_,
					        double fieldWeight_,
					        const MapAltitude &alti_,
					        const Density &_density );

private:
    //--  每次调用回调函数，都需要做的 指针重绑定 --
    inline static  Wheat_PvtBinary *rebind_ptr( GameObj &goRef_ ){
        return reinterpret_cast<Wheat_PvtBinary*>(goRef_.get_pvtBinaryPtr());
    }
                            
    static void bind( GameObj &goRef_ );
    static void rebind( GameObj &goRef_ );

    //--- callback ---//
    static void OnRenderUpdate( GameObj &goRef_ ); 
    static void OnLogicUpdate( GameObj &goRef_ ); 
    static void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );
};


}//------------- namespace gameObjs: end ----------------
#endif 

