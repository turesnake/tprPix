/*
 * ===================== PlayerGoCircle.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.19
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   
 * ----------------------------
 */
#ifndef TPR_PLAYER_GO_CIRCLE_H
#define TPR_PLAYER_GO_CIRCLE_H

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

/*
struct PlayerGoCircle_PvtBinary{
    int   tmp {};
};
*/


class PlayerGoCircle{
public:
    PlayerGoCircle() = default;

    //--- 延迟init ---//
    static void init_in_autoMod(GameObj &goRef_,
                            const ParamBinary &dyParams_ );

private:

    //--  每次调用回调函数，都需要做的 指针重绑定 --
    /*
    inline static  PlayerGoCircle_PvtBinary *rebind_ptr( GameObj &goRef_ ){
        return reinterpret_cast<PlayerGoCircle_PvtBinary*>(goRef_.get_pvtBinaryPtr());
    }
    */

    //--- callback ---//
    static void OnRenderUpdate( GameObj &goRef_ ); 
    static void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );
};


}//------------- namespace gameObjs: end ----------------
#endif 

