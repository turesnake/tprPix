/*
 * ================ Button_SceneBegin_Archive.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_BUTTON_SCENE_BEGIN_ARCHIVE_2_H
#define TPR_BUTTON_SCENE_BEGIN_ARCHIVE_2_H

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


class Button_SceneBegin_Archive{
public:
    Button_SceneBegin_Archive() = default;

    //--- 延迟init ---//
    static void init_in_autoMod( GameObj &goRef_,
                                 const ParamBinary &dyParams_ );

private:
    //--- callback ---//
    static void OnRenderUpdate( GameObj &goRef_ ); 
    static void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );
};


}//------------- namespace uiGos: end ----------------
#endif 

