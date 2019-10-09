/*
 * ================ Button_SceneBegin_Archive.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_UI_BUTTON_SCENE_BEGIN_ARCHIVE_2_H
#define TPR_UI_BUTTON_SCENE_BEGIN_ARCHIVE_2_H

//-------------------- Engine --------------------//
#include "GameObj.h"
#include "dyParams.h"

namespace uiGos{//------------- namespace uiGos ----------------


class Button_SceneBegin_Archive{
public:
    static void init( GameObj &goRef_, const DyParam &dyParams_ );
private:
    //--- callback ---//
    static void OnRenderUpdate( GameObj &goRef_ ); 
    static void OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ );
};


}//------------- namespace uiGos: end ----------------
#endif 

