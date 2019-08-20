/*
 * ==================== PlayerGoCircle.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.19
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * 
 * ----------------------------
 */
#include "Script/gameObjs/oth/PlayerGoCircle.h"

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
void PlayerGoCircle::init_in_autoMod(  goSpecId_t specID_,
                                GameObj &goRef_,
					            double fieldWeight_,
					            const MapAltitude &alti_,
					            const Density &_density ){

    //================ go.pvtBinary =================//
    //goRef_.resize_pvtBinary( sizeof(PlayerGoCircle_PvtBinary) );
    //PlayerGoCircle_PvtBinary  *pvtBp = reinterpret_cast<PlayerGoCircle_PvtBinary*>(goRef_.get_pvtBinaryPtr());


    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//

        //-- 制作 mesh 实例: "root" --
        goRef_.creat_new_goMesh("root", //- gmesh-name
                                "playerGoCircle", 
                                "selfRotate",
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_playerGoCircle_shader(),  // pic shader
                                glm::vec2{ 0.0f, 0.0f }, //- pposoff
                                0.0,  //- off_z
                                true, //- isVisible
                                false  // isCollide -- 不参加碰撞检测，也不会写到 mapent上
                                );

        //-- 制作 mesh 实例: "front" --
        goRef_.creat_new_goMesh("front", //- gmesh-name
                                "playerGoCircle", 
                                "selfRotate",
                                RenderLayerType::PlayerGoIndication, //- 固定zOff值
                                &esrc::get_playerGoCircle_shader(),  // pic shader
                                glm::vec2{ 0.0f, 0.0f }, //- pposoff
                                0.0,  //- off_z
                                true, //- isVisible
                                false  // isCollide -- 不参加碰撞检测，也不会写到 mapent上
                                );

        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &PlayerGoCircle::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &PlayerGoCircle::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &PlayerGoCircle::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Move_Idle );
    goRef_.actionSwitch.signUp( ActionSwitchType::selfRotate );

    //================ go self vals =================//

    //-- 务必在 mesh:"root" 之后 ---
    goRef_.goPos.init_currentDPos( );
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
/*
void PlayerGoCircle::bind( GameObj &goRef_ ){
}
*/

/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
/*
void PlayerGoCircle::rebind( GameObj &goRef_ ){
}
*/

/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void PlayerGoCircle::OnRenderUpdate( GameObj &goRef_ ){

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
void PlayerGoCircle::OnLogicUpdate( GameObj &goRef_ ){
    // 什么也没做...
}


/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 
 */
void PlayerGoCircle::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    //PlayerGoCircle_PvtBinary  *pvtBp = PlayerGoCircle::rebind_ptr( goRef_ );
    //=====================================//

    //-- 获得所有 goMesh 的访问权 --
    GameObjMesh &goMeshRef = goRef_.get_goMeshRef("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( type_ ){
        case ActionSwitchType::Move_Idle:
            goMeshRef.bind_animAction( "playerGoCircle", "move_idle" );
            break;

        case ActionSwitchType::selfRotate:
            goMeshRef.bind_animAction( "playerGoCircle", "selfRotate" );
            break;

        default:
            break;
            //-- 并不报错，什么也不做...

    }


}


}//------------- namespace gameObjs: end ----------------

