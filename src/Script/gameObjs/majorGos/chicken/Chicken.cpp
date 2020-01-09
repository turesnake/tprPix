/*
 * ========================= Chicken.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/majorGos/chicken/Chicken.h"

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "Density.h"
#include "animSubspeciesId.h"
#include "dyParams.h"

#include "GoSpecFromJson.h"

#include "esrc_shader.h" 
#include "esrc_gameSeed.h"
#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//


using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs {//------------- namespace gameObjs ----------------



namespace chicken_inn {//----------- namespace: chicken_inn ----------------//

    inline std::uniform_int_distribution<int>     uDistribution_int(      3,   15 );
    inline std::uniform_real_distribution<double> uDistribution_double( -1.3, 1.3 );

}//-------------- namespace: chicken_inn end ----------------//

struct Chicken_PvtBinary{
    animSubspeciesId_t subspeciesId {};
    int        tmp {};
    int        timeStep  {10};
    int        timeCount {};
    glm::dvec2 moveVec {}; //- 位移向量，确保是 标准向量
};



void Chicken::init(GameObj &goRef_, const DyParam &dyParams_ ){

    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<Chicken_PvtBinary>();
    
    //================ dyParams =================//
    size_t randUVal {};
    
    //---    
    size_t typeHash = dyParams_.get_typeHash();
    if( dyParams_.is_Nil() ){
        randUVal = 17; //- 随便写

        pvtBp->subspeciesId = esrc::apply_a_random_animSubspeciesId( "chicken.hen", AnimLabel::Default, 10 ); //- 暂时只有一个 亚种

        //----- must before creat_new_goMesh() !!! -----//
        goRef_.actionDirection.reset( apply_a_random_direction_without_mid(randUVal) ); // not Center
        goRef_.brokenLvl.reset( BrokenLvl::Lvl_0 );

    }else if( typeHash == typeid(DyParams_Blueprint).hash_code() ){
        
        const DyParams_Blueprint *bpParamPtr = dyParams_.get_binaryPtr<DyParams_Blueprint>();
        const GoDataForCreate *goDataPtr = bpParamPtr->goDataPtr;
        tprAssert( !goDataPtr->isMultiGoMesh ); // must single gomesh
        pvtBp->subspeciesId = (*goDataPtr->goMeshDataUPtrs.cbegin())->subspeciesId;

        randUVal = bpParamPtr->mapEntUWeight;    

        //----- must before creat_new_goMesh() !!! -----//
        goRef_.actionDirection.reset( goDataPtr->direction );
        goRef_.brokenLvl.reset( goDataPtr->brokenLvl );

    }else{
        tprAssert(0); //- 尚未实现
    }


    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//
        //-- 制作唯一的 mesh 实例: "root" --
        goRef_.creat_new_goMesh("root", //- gmesh-name
                                pvtBp->subspeciesId,
                                "idle",
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_shaderRef(ShaderType::UnifiedColor),  // pic shader
                                glm::dvec2{ 0.0, 0.0 }, //- pposoff
                                0.0,  //- zOff
                                true //- isVisible
                                );
        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &Chicken::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &Chicken::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &Chicken::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );
    goRef_.actionSwitch.signUp( ActionSwitchType::Move );


    //================ go self vals =================//   

}

/* -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void Chicken::bind( GameObj &goRef_ ){
}


/* -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void Chicken::rebind( GameObj &goRef_ ){
}


void Chicken::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<Chicken_PvtBinary>();

    //=====================================//
    //         更新 位移系统
    //-------------------------------------//
    goRef_.move.RenderUpdate();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.render_all_goMesh();
}



void Chicken::OnLogicUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<Chicken_PvtBinary>();

    //=====================================//
    //            AI
    //-------------------------------------//    
    if( !goRef_.isControlByPlayer ){
        //-- 简单的随机游走
        pvtBp->timeCount++;


        if( pvtBp->timeStep == 0 ){
            pvtBp->timeStep = 4;
        }

        if( (pvtBp->timeCount % pvtBp->timeStep) == 0 ){

            auto &engine = esrc::get_gameSeed().getnc_realRandEngine();

            pvtBp->timeStep = chicken_inn::uDistribution_int(engine) + 1; //- 每帧都更新 间隔值

            glm::dvec2 randVec {chicken_inn::uDistribution_double(engine),
                                chicken_inn::uDistribution_double(engine) };

            pvtBp->moveVec = glm::normalize( pvtBp->moveVec + randVec );
        }

        //-- 确保每一帧都位移，但只在一段时间后 才修改 位移方向 --
        DirAxes dirAxes {pvtBp->moveVec};
        goRef_.move.set_newCrawlDirAxes( dirAxes );
    }

}



void Chicken::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){


    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<Chicken_PvtBinary>();
    //=====================================//

    auto dir = goRef_.actionDirection.get_newVal();
    auto brokenLvl = goRef_.brokenLvl.get_newVal();

    //-- 获得所有 goMesh 的访问权 --
    GameObjMesh &goMeshRef = goRef_.get_goMeshRef("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( type_ ){
        case ActionSwitchType::Idle:
            goMeshRef.bind_animAction( pvtBp->subspeciesId, dir, brokenLvl, "idle" );
            goRef_.rebind_rootAnimActionPosPtr(); //- 临时性的方案 ...
            break;

        case ActionSwitchType::Move:

                // walk, run, fly
                // 使用一个 统一的函数 来管理
                // ...

            //goMeshRef.bind_animAction( pvtBp->subspeciesId, dir, brokenLvl, "fly" );
            //goRef_.rebind_rootAnimActionPosPtr(); //- 临时性的方案 ...
                
                // 这两句话，也应该被直接放进 moveState_manage 函数中去


                Chicken::moveState_manage(  goRef_, goMeshRef );



            break;

        default:
            break;
            //-- 并不报错，什么也不做...
    }

}


//
//
//
// 临时性的，非常没效率的 写法 ......
//
//  目前的 bug 在于，提升 moveSpeedLvl, 并不会 调用本函数
//
//
void Chicken::moveState_manage( GameObj &goRef_,
                                GameObjMesh &goMeshRef_
                                )noexcept{


    auto dir = goRef_.actionDirection.get_newVal();
    auto brokenLvl = goRef_.brokenLvl.get_newVal();
    auto moveSpeedLvl = goRef_.moveSpeedLvl.get_newVal();

    auto *pvtBp = goRef_.get_pvtBinaryPtr<Chicken_PvtBinary>();


    goSpeciesId_t chickenId = GoSpecFromJson::str_2_goSpeciesId("chicken");
    GoSpecFromJson &goSpecFromJsonRef = GoSpecFromJson::getnc_goSpecFromJsonRef( chickenId );

    const auto &moveStateTable = *(goSpecFromJsonRef.moveStateTableUPtr);

    // 当 go moveSpeedLvl 越界时，不报错，而是强行改回 区间内
    // minLvl, maxLvl
    if( moveSpeedLvl < moveStateTable.minLvl ){
        moveSpeedLvl = moveStateTable.minLvl;
        goRef_.moveSpeedLvl.set_newVal( moveSpeedLvl ); // 可能会让 数据 dirty ...
    }else if( moveSpeedLvl > moveStateTable.maxLvl ){
        moveSpeedLvl = moveStateTable.maxLvl;
        goRef_.moveSpeedLvl.set_newVal( moveSpeedLvl ); // 可能会让 数据 dirty ...
    }


    // 最简单的映射，并不做加减速 .....
    //
    auto it = moveStateTable.table.find(moveSpeedLvl);
    tprAssert( it != moveStateTable.table.end() );
    const std::string &actionName = it->second;

        
        cout << "speedLvl: " << static_cast<int>(moveSpeedLvl)
            << "; actionName: " << actionName 
            << endl;
        

    goMeshRef_.bind_animAction( pvtBp->subspeciesId, dir, brokenLvl, actionName );
    goRef_.rebind_rootAnimActionPosPtr(); //- 临时性的方案 ...






}


}//------------- namespace gameObjs: end ----------------

