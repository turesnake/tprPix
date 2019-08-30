/*
 * ========================= OneEyeBoy.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * 
 * ----------------------------
 */
#include "Script/gameObjs/majorGos/OneEyeBoy.h"

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_shader.h" 

//-------------------- Script --------------------//
#include "Script/resource/ssrc.h" 

using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs {//------------- namespace gameObjs ----------------



/* ===========================================================
 *                   init_in_autoMod
 * -----------------------------------------------------------
 */
void OneEyeBoy::init_in_autoMod(GameObj &goRef_,
                                const ParamBinary &dyParams_ ){

    //================ go.pvtBinary =================//
    goRef_.resize_pvtBinary( sizeof(OneEyeBoy_PvtBinary) );
    OneEyeBoy_PvtBinary  *pvtBp = reinterpret_cast<OneEyeBoy_PvtBinary*>(goRef_.get_pvtBinaryPtr());


    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//
        //-- 制作唯一的 mesh 实例: "root" --
        goRef_.creat_new_goMesh("root", //- gmesh-name
                                "oneEyeBoy", 
                                "move_idle",
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_rect_shader(),  // pic shader
                                glm::vec2{ 0.0f, 0.0f }, //- pposoff
                                0.0,  //- off_z
                                true //- isVisible
                                );
        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &OneEyeBoy::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &OneEyeBoy::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &OneEyeBoy::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Move_Idle );
    goRef_.actionSwitch.signUp( ActionSwitchType::Move_Move );


    //================ go self vals =================//

    //-- 务必在 mesh:"root" 之后 ---
    goRef_.init_goPos_currentDPos( );
    //...    

    //--- MUST ---
    goRef_.init_check();
}

/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void OneEyeBoy::bind( GameObj &goRef_ ){
}


/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void OneEyeBoy::rebind( GameObj &goRef_ ){
}

/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void OneEyeBoy::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    OneEyeBoy_PvtBinary  *pvtBp = OneEyeBoy::rebind_ptr( goRef_ );

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
 *                        OnLogicUpdate
 * -----------------------------------------------------------
 */
void OneEyeBoy::OnLogicUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    OneEyeBoy_PvtBinary  *pvtBp = OneEyeBoy::rebind_ptr( goRef_ );
    //=====================================//

    // 什么也没做...
}


/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 
 */
void OneEyeBoy::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    OneEyeBoy_PvtBinary  *pvtBp = OneEyeBoy::rebind_ptr( goRef_ );
    //=====================================//

    //-- 获得所有 goMesh 的访问权 --
    GameObjMesh &goMeshRef = goRef_.get_goMeshRef("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( type_ ){
        case ActionSwitchType::Move_Idle:
            goMeshRef.bind_animAction( "oneEyeBoy", "move_idle" );
            break;

        //case ActionSwitchType::Move_Move:
        //    goMeshRef.bind_animAction( "oneEyeBoy", "move_walk" );
        //    break;

        default:
            break;
            //-- 并不报错，什么也不做...

    }


}


}//------------- namespace gameObjs: end ----------------

