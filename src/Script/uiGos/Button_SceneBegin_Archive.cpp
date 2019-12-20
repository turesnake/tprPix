/*
 * =============== Button_SceneBegin_Archive.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * 
 * ----------------------------
 */
#include "Script/uiGos/Button_SceneBegin_Archive.h"

//-------------------- CPP --------------------//
#include <functional>
#include <string>
#include <vector>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "Density.h"
#include "animSubspecId.h"

#include "esrc_shader.h" 
#include "esrc_player.h"
#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//

using namespace std::placeholders;

#include "tprDebug.h" 


namespace uiGos {//------------- namespace uiGos ----------------



struct Button_SceneBegin_Archive_PvtBinary{
    animSubspecId_t subspecId {};
    int   tmp {};
};


/* ===========================================================
 *                   init
 * -----------------------------------------------------------
 */
void Button_SceneBegin_Archive::init(GameObj &goRef_,const DyParam &dyParams_ ){

    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<Button_SceneBegin_Archive_PvtBinary>();

    pvtBp->subspecId = esrc::apply_a_random_animSubspecId( "button_beginScene", emptyAnimLabels, 10 );

    //----- must before creat_new_goMesh() !!! -----//
    goRef_.set_actionDirection( NineDirection::Mid );

    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//
        //-- 制作 mesh 实例: "root" --
        GameObjMesh &rootGoMesh = goRef_.creat_new_goMesh(
                                "root", //- gmesh-name
                                pvtBp->subspecId, 
                                "new", 
                                RenderLayerType::UIs, //- 固定zOff值  
                                &esrc::get_shaderRef(ShaderType::OriginColor),  // pic shader
                                glm::vec2{ 0.0f, 0.0f }, //- pposoff
                                0.0,  //- zOff， 沉在所有 MajorGo 后方
                                true //- isVisible
                                );
        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &Button_SceneBegin_Archive::OnRenderUpdate,  _1 );   

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &Button_SceneBegin_Archive::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::ButtonState_1 );
    goRef_.actionSwitch.signUp( ActionSwitchType::ButtonState_2 );

    //================ go self vals =================//

    //...    
}


/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void Button_SceneBegin_Archive::OnRenderUpdate( GameObj &goRef_ ){

    //=====================================//
    //            AI
    //-------------------------------------//
    //...

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
void Button_SceneBegin_Archive::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

            cout << "Button_SceneBegin_Archive::OnActionSwitch" << endl;

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<Button_SceneBegin_Archive_PvtBinary>();

    //-- 获得所有 goMesh 的访问权 --
    GameObjMesh &goMeshRef = goRef_.get_goMeshRef("root");

    //-- 处理不同的 actionSwitch 分支 --

    switch( type_ ){
        case ActionSwitchType::ButtonState_1:
            goMeshRef.bind_animAction( pvtBp->subspecId, goRef_.get_actionDirection(), "new" );
            break;

        case ActionSwitchType::ButtonState_2:
            goMeshRef.bind_animAction( pvtBp->subspecId, goRef_.get_actionDirection(), "data" );
            break;

        default:
            break;
            //-- 并不报错，什么也不做...

    }
    //goRef_.rebind_rootAnimActionPosPtr(); //- 临时性的方案 ...
}


}//------------- namespace uiGos: end ----------------

