/*
 * ========================= Dog_A.cpp ==========================
 *                          -- tpr --
 *                                        创建 -- 2018.12.23
 *                                        修改 -- 2018.12.23
 * ----------------------------------------------------------
 *   具象GameObj类 示范。
 * 
 * ----------------------------
 */
#include "Script/gameObjs/Dog_A.h"

//-------------------- C --------------------//
#include <cassert> //- assert

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Engine --------------------//
#include "srcs_engine.h" 

//-------------------- Script --------------------//
#include "Script/actionHandle/Cycle.h"
#include "Script/resource/srcs_script.h" 

using namespace std::placeholders;

using std::string;

//#include "debug.h" //- tmp


namespace gameObjs{//------------- namespace gameObjs ----------------


//---------- static ----------//
u32  Dog_A::specId {0};



namespace{//-------------- namespace ------------------//

    int step_x_count {0};

    FlyIns  flyIns { -3.14f };


}//------------------ namespace: end ------------------//

/* ===========================================================
 *                         init
 * -----------------------------------------------------------
 * -- 应该在 调用 init 之前，创建 基础go实例（并添加到 全局容器中）
 * -- 尽量把 具象go类 做得 “工厂化” 一些。
 */
void Dog_A::init( GameObj *_goPtr ){

    assert( _goPtr != nullptr );
    goPtr = _goPtr;

    //-------- bind callback funcs ---------//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goPtr->RenderUpdate = std::bind( &Dog_A::RenderUpdate, &dog_a, _1 );   
    goPtr->LogicUpdate  = std::bind( &Dog_A::LogicUpdate,  &dog_a, _1 );
    goPtr->BeAffect     = std::bind( &Dog_A::BeAffect,     &dog_a, _1 ); 

    //-------- go self vals ---------//
    goPtr->species = Dog_A::specId;
    goPtr->family = GameObjFamily::Major;
    goPtr->is_top_go = true;
    goPtr->id_parent = NULLID;
    goPtr->is_active = true;
    goPtr->state = GameObjState::Waked;
    goPtr->moveState = GameObjMoveState::Movable;
    goPtr->weight = 5.0f;
    goPtr->is_dirty = false;
    goPtr->is_control_by_player = false;

    goPtr->move.set_speedLv( SpeedLevel::LV_6 );

    goPtr->move.set_MoveType( true ); //- tmp

    //-------- action／actionHandle/ gameMesh ---------//

        //-- 制作唯一的 mesh 实例 --
        GameMesh *meshPtr = goPtr->creat_new_gameMesh();
        meshPtr->set_shader_program( &esrc::rect_shader );
        meshPtr->init( goPtr ); 
        meshPtr->is_visible = true;
        //-- bind action / actionHandle --
        meshPtr->bind_action( "human_1" );
        actionHdle::cycle_obj.bind( &(meshPtr->actionHandle), 
                                    meshPtr->get_totalFrames(), //- 画面帧总数
                                    0,                //- 起始画面帧序号
                                    6 );              //- 画面帧间 时长
        //-- oth vals --
        meshPtr->pposOff = glm::vec2{ 0.0f, 0.0f }; //- 此 gameMesh 在 go 中的 坐标偏移 
      

    //-------- go.binary ---------//
    goPtr->binary.resize( sizeof(Dog_A_Binary) );
    bp = (Dog_A_Binary*)&(goPtr->binary.at(0)); //- 绑定到本地指针

    bp->HP = 100;
    bp->MP = 95;
}



/* ===========================================================
 *                       bind
 * -----------------------------------------------------------
 * -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void Dog_A::bind( GameObj *_goPtr ){
}


/* ===========================================================
 *                       rebind
 * -----------------------------------------------------------
 * -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void Dog_A::rebind( GameObj *_goPtr ){
}


/* ===========================================================
 *                      RenderUpdate
 * -----------------------------------------------------------
 */
void Dog_A::RenderUpdate( GameObj *_goPtr ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    assert( _goPtr->species == Dog_A::specId );
    //-- rebind ptr -----
    goPtr = _goPtr;
    bp = (Dog_A_Binary*)&(goPtr->binary.at(0));

    //=====================================//
    //           test: AI
    //-------------------------------------//
    //...
    /*
    if( goPtr->is_control_by_player == false  ){

        if( step_x_count > 2 ){
            step_x_count = 0;
        }

        if( step_x_count == 0 ){

            flyIns.rad += 0.5f;
            //goPtr->move.set_newflyIns( flyIns );
        }
        step_x_count++;
    }
    */
    
    //=====================================//
    //         更新 位移系统
    //-------------------------------------//
    goPtr->move.RenderUpdate();


    //=====================================//
    //  将 确认要渲染的 gameMeshs，添加到 renderPool          
    //-------------------------------------//
    for( auto &meshRef : goPtr->gameMeshs ){

        //-- 也许不该放在 这个位置 --
        if( meshRef.is_visible == false ){
            continue;
        }

        //=== 传参到 scriptBuf : [无参数] ===
        meshRef.actionHandle.funcs.at("update")(&(meshRef.actionHandle), 0);
        //=== 从 scriptBuf 取返回值 : [无返回值] ===

        meshRef.refresh_translate();
        meshRef.refresh_scale_auto(); //- 没必要每帧都执行

        esrc::renderPool.insert({ meshRef.get_render_z(), (GameMesh*)&meshRef }); 
    }
}


/* ===========================================================
 *                        LogicUpdate
 * -----------------------------------------------------------
 */
void Dog_A::LogicUpdate( GameObj *_goPtr ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    assert( _goPtr->species == Dog_A::specId );
    //-- rebind ptr -----
    goPtr = _goPtr;
    bp = (Dog_A_Binary*)&(goPtr->binary.at(0));
    //=====================================//

    // 什么也没做...

    
    
    

}



/* ===========================================================
 *                       BeAffect
 * -----------------------------------------------------------
 */
void Dog_A::BeAffect( GameObj *_goPtr ){
}





/* ===========================================================
 *                   move_to_target
 * -----------------------------------------------------------
 * --  为爬行go确定一个 目标pos，让它朝着那个方向移动。
 */
void Dog_A::move_to_target(){

    //-- 假定一个 targetPos 
    glm::vec2  targetPos { 200.0f, 35.0f };

	
}




}//------------- namespace gameObjs: end ----------------

