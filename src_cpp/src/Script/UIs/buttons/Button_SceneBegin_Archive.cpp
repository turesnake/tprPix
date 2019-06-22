/*
 * =========== Button_SceneBegin_Archive.cpp ================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/UIs/buttons/Button_SceneBegin_Archive.h"

//-------------------- C --------------------//
#include <cmath>

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_shader.h" 

//-------------------- Script --------------------//
#include "Script/resource/ssrc.h" 

using namespace std::placeholders;

//#include "tprDebug.h" 

namespace uis{//------------- namespace uis ----------------

/* ===========================================================
 *                  init_in_autoMod
 * -----------------------------------------------------------
 */
void Button_SceneBegin_Archive::init_in_autoMod(  UIObj  *_uiObjPtr,
                                                const glm::vec2 &_fpos ){

    tprAssert( _uiObjPtr != nullptr );
    this->uiObjPtr = _uiObjPtr;

    //-------- bind callback funcs ---------//
    // 暂无...
    //-------- actionSwitch ---------//
    // 暂无...

    //-------- go self vals ---------//
    this->uiObjPtr->uiObjSpecId = Button_SceneBegin_Archive::specId;
    this->uiObjPtr->skip_to_fpos( _fpos );

    //-------- animFrameSet／animFrameIdxHandle/ goMesh ---------//

        //-- 制作唯一的 mesh 实例: "root" --
        UIMesh &uiMeshRef = this->uiObjPtr->creat_new_uiMesh(
                                        &esrc::get_rect_shader(),  
                                        &esrc::get_rect_shader(), //- 其实没有 shadow
                                        1.0,  //- off_z 通过此值来确保，archive 在 pointer 后方
                                        true, //- isVisible
                                        false //- isFlipOver
                                        );

        uiMeshRef.bind_animAction(  "button_beginScene", "new" );
}



}//------------- namespace uis: end ----------------

