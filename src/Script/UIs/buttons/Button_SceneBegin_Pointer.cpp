/*
 * =========== Button_SceneBegin_Pointer.cpp ================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/UIs/buttons/Button_SceneBegin_Pointer.h"

//-------------------- C --------------------//
#include <cassert>
#include <cmath>

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Engine --------------------//
#include "esrc_shader.h" 

//-------------------- Script --------------------//
#include "Script/resource/ssrc.h" 


using namespace std::placeholders;

//#include "debug.h" 

namespace uis{//------------- namespace uis ----------------

/* ===========================================================
 *                  init_in_autoMod
 * -----------------------------------------------------------
 */
void Button_SceneBegin_Pointer::init_in_autoMod(  UIObj  *_uiObjPtr,
                                                const glm::vec2 &_fpos ){

    assert( _uiObjPtr != nullptr );
    this->uiObjPtr = _uiObjPtr;

    //-------- bind callback funcs ---------//
    // 暂无...
    //-------- actionSwitch ---------//
    // 暂无...

    //-------- go self vals ---------//
    this->uiObjPtr->uiObjSpecId = Button_SceneBegin_Pointer::specId;

    this->uiObjPtr->skip_to_fpos( _fpos );
    //-------- animFrameSet／animFrameIdxHandle/ goMesh ---------//

        //-- 制作唯一的 mesh 实例: "root" --
        UIMesh &uiMeshRef = this->uiObjPtr->creat_new_uiMesh(
                                        "button_beginScene", //- animFrameSet-Name
                                        &esrc::rect_shader,  
                                        &esrc::rect_shader, //- 其实没有 shadow
                                        1.1,  //- off_z  通过此值来确保，archive 在 pointer 后方
                                        true, //- isVisible
                                        false //- isFlipOver
                                        );

        //-- bind animFrameSet / animFrameIdxHandle --
        uiMeshRef.getnc_animFrameIdxHandle().bind_idle( 0 ); //- "pointer"
}


}//------------- namespace uis: end ----------------

