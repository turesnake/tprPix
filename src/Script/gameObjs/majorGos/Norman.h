/*
 * ========================= Norman.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   诺曼人
 * ----------------------------
 */
#ifndef TPR_NORMAN_H
#define TPR_NORMAN_H

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

struct Norman_PvtBinary{
    int   tmp  {};
};


class Norman{
public:
    Norman() = default;

    //--- 延迟init ---//
    static void init_in_autoMod(   goSpecId_t specID_,
                            GameObj &goRef_,
					        double fieldWeight_,
					        const MapAltitude &alti_,
					        const Density &_density );

private:
    //--  每次调用回调函数，都需要做的 指针重绑定 --
    inline static Norman_PvtBinary *rebind_ptr( GameObj &goRef_ ){
        return reinterpret_cast<Norman_PvtBinary*>(goRef_.get_pvtBinaryPtr());
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

