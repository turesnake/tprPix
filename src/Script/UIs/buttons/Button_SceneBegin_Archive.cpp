/*
 * =========== Button_SceneBegin_Archive.cpp ================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/UIs/buttons/Button_SceneBegin_Archive.h"

//-------------------- C --------------------//
#include <cassert> //- assert
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
void Button_SceneBegin_Archive::init_in_autoMod(  UIObj  *_uiObjPtr,
                                                const glm::vec2 &_fpos ){

    assert( _uiObjPtr != nullptr );
    this->uiObjPtr = _uiObjPtr;


    //-------- bind callback funcs ---------//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    this->uiObjPtr->RenderUpdate = std::bind( &Button_SceneBegin_Archive::OnRenderUpdate, &button_sceneBegin_archive, _1 );   

    
    //-------- actionSwitch ---------//
    // 暂无...

    //-------- go self vals ---------//
    this->uiObjPtr->uiObjSpecId = Button_SceneBegin_Archive::specId;

    this->uiObjPtr->skip_to_fpos( _fpos );

    //-------- animFrameSet／animFrameIdxHandle/ goMesh ---------//

        //-- 制作唯一的 mesh 实例: "root" --
        UIMesh &uiMeshRef = this->uiObjPtr->creat_new_uiMesh(
                                        "button_beginScene", //- animFrameSet-Name
                                        &esrc::rect_shader,  
                                        &esrc::rect_shader, //- 其实没有 shadow
                                        1.0,  //- off_z 通过此值来确保，archive 在 pointer 后方
                                        true, //- isVisible
                                        false //- isFlipOver
                                        );

        //-- bind animFrameSet / animFrameIdxHandle --
        uiMeshRef.getnc_animFrameIdxHandle().bind_idle( 1 ); //- "new"
}


/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void Button_SceneBegin_Archive::OnRenderUpdate( UIObj *_uiObjPtr ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    rebind_ptr( _uiObjPtr );

    //=====================================//
    //         更新 位移系统
    //-------------------------------------//
    this->uiObjPtr->renderUpdate_move();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    this->uiObjPtr->uiMesh_renderUpdate();
}


}//------------- namespace uis: end ----------------

