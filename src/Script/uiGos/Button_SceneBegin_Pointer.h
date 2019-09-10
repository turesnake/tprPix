/*
 * ================ Button_SceneBegin_Pointer.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_BUTTON_SCENE_BEGIN_POINTER_2_H
#define TPR_BUTTON_SCENE_BEGIN_POINTER_2_H

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
#include "animSubspeciesId.h"


namespace uiGos{//------------- namespace uiGos ----------------


struct Button_SceneBegin_Pointer_PvtBinary{
    animSubspeciesId_t subspeciesId {};
    int   tmp {};
};

class Button_SceneBegin_Pointer{
public:
    Button_SceneBegin_Pointer() = default;

    //--- 延迟init ---//
    static void init_in_autoMod( GameObj &goRef_,
                                 const ParamBinary &dyParams_ );

private:
    //--  每次调用回调函数，都需要做的 指针重绑定 --
    inline static  Button_SceneBegin_Pointer_PvtBinary *rebind_ptr( GameObj &goRef_ ){
        return reinterpret_cast<Button_SceneBegin_Pointer_PvtBinary*>(goRef_.get_pvtBinaryPtr());
    }
    //--- callback ---//
    static void OnRenderUpdate( GameObj &goRef_ ); 
    static void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );
};


}//------------- namespace uiGos: end ----------------
#endif 

