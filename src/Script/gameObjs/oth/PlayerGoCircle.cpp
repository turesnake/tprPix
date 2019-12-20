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


namespace gameObjs {//------------- namespace gameObjs ----------------


struct PlayerGoCircle_PvtBinary{
    animSubspecId_t subspecId {};
    int   tmp {};
};


/* ===========================================================
 *                   init
 * -----------------------------------------------------------
 */
void PlayerGoCircle::init(GameObj &goRef_,const DyParam &dyParams_ ){

    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<PlayerGoCircle_PvtBinary>();

    pvtBp->subspecId = esrc::apply_a_random_animSubspecId( "playerGoCircle", emptyAnimLabels, 10 );

    //----- must before creat_new_goMesh() !!! -----//
    goRef_.set_actionDirection( NineDirection::Mid );

    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//

        //-- 制作 mesh 实例: "root" --
        GameObjMesh &rootGoMesh = goRef_.creat_new_goMesh(
                                "root", //- gmesh-name
                                pvtBp->subspecId,
                                "idle", 
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值  
                                &esrc::get_shaderRef(ShaderType::PlayerGoCircle),  // pic shader
                                glm::dvec2{ 0.0, 0.0 }, //- pposoff
                                -500.0,  //- zOff， 沉在所有 MajorGo 后方
                                true //- isVisible
                                );
        /*
        //-- 制作 mesh 实例: "front" --
        GameObjMesh &frontGoMesh = goRef_.creat_new_goMesh(
                                "front", //- gmesh-name
                                "playerGoCircle", 
                                "idle",
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值  
                                &esrc::get_shaderRef(ShaderType::PlayerGoCircle),  // pic shader
                                glm::dvec2{ 0.0, 0.0 }, //- pposoff
                                500.0,  //- zOff， 浮在所有 MajorGo 前方
                                true //- isVisible
                                );
        */

        rootGoMesh.rotateScaleData.reset_rotateOrder( std::vector<RotateType>{ RotateType::X, RotateType::Z } ); //- 只需设置一次
        rootGoMesh.rotateScaleData.set_rotateDegree( glm::vec3( 40.0f, 0.0f, 0.0f ) );

        rootGoMesh.rotateScaleData.set_scale( glm::vec2( 0.3f, 0.3f ) );
        

        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &PlayerGoCircle::OnRenderUpdate,  _1 );   

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &PlayerGoCircle::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );
    goRef_.actionSwitch.signUp( ActionSwitchType::selfRotate );

    //================ go self vals =================//
    
    //...    

}


/* ===========================================================
 *                      OnRenderUpdate
 * -----------------------------------------------------------
 */
void PlayerGoCircle::OnRenderUpdate( GameObj &goRef_ ){

    //=====================================//
    //            AI
    //-------------------------------------//
    //...
    
    GameObj &playerGoRef = esrc::get_player().get_goRef();

    goRef_.move.set_drag_targetDPos( playerGoRef.get_dpos() );


    //-- 每1渲染帧，手动旋转 circle 一个小角度 --
    auto &rootGoMeshRef = goRef_.get_goMeshRef("root");
    glm::vec3 rotateDegree = rootGoMeshRef.rotateScaleData.get_rotateDegreeRef();
    rotateDegree.z += 3.0f;
    if( rotateDegree.z > 360.0f ){
        rotateDegree.z -= 360.f;
    }
    rootGoMeshRef.rotateScaleData.set_rotateDegree( rotateDegree );
    



    //=====================================//
    //         更新 位移系统
    //-------------------------------------//
    goRef_.move.RenderUpdate();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.render_all_goMesh();

                    //-- 改为 调用 gomesh 的 专用 RenderUpdate 函数，可以定制 缩放的那种 

}


/* ===========================================================
 *               OnActionSwitch
 * -----------------------------------------------------------
 * -- 
 */
void PlayerGoCircle::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

        cout << "PlayerGoCircle::OnActionSwitch" << endl;

    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<PlayerGoCircle_PvtBinary>();
    //=====================================//

    //-- 获得所有 goMesh 的访问权 --
    GameObjMesh &goMeshRef = goRef_.get_goMeshRef("root");

    //-- 处理不同的 actionSwitch 分支 --

            // playerGoCircle 只有 1-frame 图元，依赖 opengl 自身动画机制。
            // 所以，当外部传入 动画指令，不能依赖动画帧，
            // 要用 opengl 自身的方式去实现动画
            // 未完成...

    switch( type_ ){
        case ActionSwitchType::Idle:
            goMeshRef.bind_animAction( pvtBp->subspecId, goRef_.get_actionDirection(), "idle" );
            break;

        case ActionSwitchType::selfRotate:
            goMeshRef.bind_animAction( pvtBp->subspecId, goRef_.get_actionDirection(), "selfRotate" );
            break;

        default:
            break;
            //-- 并不报错，什么也不做...

    }
    //goRef_.rebind_rootAnimActionPosPtr(); //- 临时性的方案 ...


}


}//------------- namespace gameObjs: end ----------------

