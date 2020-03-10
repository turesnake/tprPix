/*
 * =============== Button_SceneBegin_Archive.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.25
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
 #include "pch.h"
#include "Script/uiGos/Button_SceneBegin_Archive.h"


//-------------------- Engine --------------------//
#include "animSubspeciesId.h"

#include "esrc_player.h"
#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//

using namespace std::placeholders;



namespace uiGos {//------------- namespace uiGos ----------------



struct Button_SceneBegin_Archive_PvtBinary{
    int   tmp {};
};


/* ===========================================================
 *                   init
 * -----------------------------------------------------------
 */
void Button_SceneBegin_Archive::init(GameObj &goRef_,const DyParam &dyParams_ ){

    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<Button_SceneBegin_Archive_PvtBinary>();

    animSubspeciesId_t subspeciesId = esrc::apply_a_random_animSubspeciesId( "button_beginScene", "", 10 );

    //----- must before creat_new_goMesh() !!! -----//
    goRef_.actionDirection.reset( NineDirection::Center );
    goRef_.brokenLvl.reset( BrokenLvl::Lvl_0 );


    goRef_.set_colliDataFromJsonPtr( ColliDataFromJson_Nil::nillInstance.get() );
                                            // 丑陋的实现 
                                            

    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//
        //-- 制作 mesh 实例: "root" --
        GameObjMesh &rootGoMesh = goRef_.goMeshSet.creat_new_goMesh(
                                "root", //- gmesh-name
                                subspeciesId, 
                                AnimActionEName::New, 
                                RenderLayerType::UIs, //- 固定zOff值  
                                ShaderType::OriginColor,  // pic shader
                                glm::vec2{ 0.0f, 0.0f }, //- pposoff
                                0.0,  //- zOff， 沉在所有 MajorGo 后方
                                1151, // uweight tmp
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
    goRef_.goMeshSet.render_all_goMeshs_without_callback();
}


/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 
 */
void Button_SceneBegin_Archive::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<Button_SceneBegin_Archive_PvtBinary>();

    auto dir = goRef_.actionDirection.get_newVal();
    auto brokenLvl = goRef_.brokenLvl.get_newVal();

    //-- 获得所有 goMesh 的访问权 --
    GameObjMesh &goMeshRef = goRef_.goMeshSet.get_goMeshRef("root");

    //-- 处理不同的 actionSwitch 分支 --

    switch( type_ ){
        case ActionSwitchType::ButtonState_1:
            goMeshRef.set_animActionEName( AnimActionEName::New );
            goMeshRef.bind_animAction();
            break;

        case ActionSwitchType::ButtonState_2:
            goMeshRef.set_animActionEName( AnimActionEName::Data );
            goMeshRef.bind_animAction();
            break;

        default:
            break;
            //-- 并不报错，什么也不做...

    }
    //goRef_.rebind_rootAnimActionPosPtr(); //- 临时性的方案 ...
}


}//------------- namespace uiGos: end ----------------

