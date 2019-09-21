/*
 * ========================= WallA.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.01.30
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * 
 * ----------------------------
 */
#include "Script/gameObjs/majorGos/WallA.h"

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "esrc_shader.h" 
#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//
#include "Script/resource/ssrc.h" 

using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs {//------------- namespace gameObjs ----------------


struct WallA_PvtBinary{
    animSubspeciesId_t subspeciesId {};
    int   tmp {};
};


/* ===========================================================
 *                   init_in_autoMod
 * -----------------------------------------------------------
 */
void WallA::init_in_autoMod(GameObj &goRef_,
                                const ParamBinary &dyParams_ ){

    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<WallA_PvtBinary>();

    pvtBp->subspeciesId = esrc::apply_a_random_animSubspeciesId( "wallA", emptyAnimLabels, 10 ); //- 暂时只有一个 亚种

    //----- must before creat_new_goMesh() !!! -----//
    goRef_.set_direction( NineDirection::Mid );

    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//
        //-- 制作唯一的 mesh 实例: "root" --
        goRef_.creat_new_goMesh("root", //- gmesh-name
                                pvtBp->subspeciesId,
                                "idle",
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_rect_shader(),  // pic shader
                                glm::vec2{ 0.0f, 0.0f }, //- pposoff
                                0.0,  //- off_z
                                true //- isVisible
                                );
        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &WallA::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &WallA::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &WallA::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );


    //================ go self vals =================//   

}

/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void WallA::bind( GameObj &goRef_ ){
}


/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void WallA::rebind( GameObj &goRef_ ){
}

/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void WallA::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<WallA_PvtBinary>();

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
void WallA::OnLogicUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<WallA_PvtBinary>();
    //=====================================//

    // 什么也没做...
}


/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 
 */
void WallA::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

        cout << "WallA::OnActionSwitch" << endl;
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<WallA_PvtBinary>();
    //=====================================//

    //-- 获得所有 goMesh 的访问权 --
    GameObjMesh &goMeshRef = goRef_.get_goMeshRef("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( type_ ){
        case ActionSwitchType::Idle:
            goMeshRef.bind_animAction( pvtBp->subspeciesId, goRef_.get_direction(), "idle" );
            break;

        //case ActionSwitchType::Move_Move:
        //    goMeshRef.bind_animAction( pvtBp->subspeciesId, "move_walk" );
        //    break;

        default:
            break;
            //-- 并不报错，什么也不做...

    }

    //goRef_.rebind_rootAnimActionPosPtr(); //- 临时性的方案 ...


}


}//------------- namespace gameObjs: end ----------------

