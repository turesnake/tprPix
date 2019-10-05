/*
 * =============== Button_SceneBegin_Pointer.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/uiGos/Button_SceneBegin_Pointer.h"

//-------------------- CPP --------------------//
#include <functional>
#include <string>
#include <vector>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_shader.h" 
#include "esrc_player.h"
#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//
#include "Script/resource/ssrc.h" 

using namespace std::placeholders;

#include "tprDebug.h" 


namespace uiGos {//------------- namespace uiGos ----------------


struct Button_SceneBegin_Pointer_PvtBinary{
    animSubspeciesId_t subspeciesId {};
    int   tmp {};
};


/* ===========================================================
 *                   init_in_autoMod
 * -----------------------------------------------------------
 */
void Button_SceneBegin_Pointer::init_in_autoMod(GameObj &goRef_,
                                        const DyParam &dyParams_ ){

    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<Button_SceneBegin_Pointer_PvtBinary>();

    pvtBp->subspeciesId = esrc::apply_a_random_animSubspeciesId( "button_beginScene", emptyAnimLabels, 10 );


    //----- must before creat_new_goMesh() !!! -----//
    goRef_.set_actionDirection( NineDirection::Mid );

    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//

        //-- 制作 mesh 实例: "root" --
        GameObjMesh &rootGoMesh = goRef_.creat_new_goMesh(
                                "root", //- gmesh-name
                                pvtBp->subspeciesId,
                                "pointer", 
                                RenderLayerType::UIs, //- 固定zOff值  
                                &esrc::get_shaderRef(ShaderType::OriginColor),  // pic shader
                                glm::vec2{ 0.0f, 0.0f }, //- pposoff
                                0.5,  //- off_z，确保，浮在 archive图元 上方
                                true //- isVisible
                                );
        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &Button_SceneBegin_Pointer::OnRenderUpdate,  _1 );   

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &Button_SceneBegin_Pointer::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::ButtonState_1 );

    //================ go self vals =================//

    //...    
}


/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void Button_SceneBegin_Pointer::OnRenderUpdate( GameObj &goRef_ ){

    //=====================================//
    //         更新 位移系统
    //-------------------------------------//
    goRef_.move.RenderUpdate();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.render_all_goMesh();
}


/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 
 */
void Button_SceneBegin_Pointer::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

            cout << "Button_SceneBegin_Pointer::OnActionSwitch" << endl;

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<Button_SceneBegin_Pointer_PvtBinary>();

    //-- 获得所有 goMesh 的访问权 --
    GameObjMesh &goMeshRef = goRef_.get_goMeshRef("root");

    //-- 处理不同的 actionSwitch 分支 --

    switch( type_ ){
        case ActionSwitchType::ButtonState_1:
            goMeshRef.bind_animAction( pvtBp->subspeciesId, goRef_.get_actionDirection(),  "pointer" );
            break;
        default:
            break;
            //-- 并不报错，什么也不做...
    }
    //goRef_.rebind_rootAnimActionPosPtr(); //- 临时性的方案 ...
}


}//------------- namespace uiGos: end ----------------

