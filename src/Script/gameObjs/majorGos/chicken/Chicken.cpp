/*
 * ========================= Chicken.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Script/gameObjs/majorGos/chicken/Chicken.h"

//-------------------- Engine --------------------//
#include "animSubspeciesId.h"
#include "dyParams.h"
#include "GoSpecFromJson.h"
#include "assemble_go.h"

#include "esrc_gameSeed.h"

//-------------------- Script --------------------//


using namespace std::placeholders;


namespace gameObjs {//------------- namespace gameObjs ----------------


struct Chicken_PvtBinary{
    int        tmp {};
    int        timeStep  {10};
    int        timeCount {};
    glm::dvec2 moveVec {}; //- 位移向量，确保是 标准向量


    bool    isFlying        {false};
    bool    isShakeFlying   {false};
    size_t  flyCount        {0}; // every renderFrame +1
    double  shakeFlyCount   {0.0}; // every renderFrame +1

};


namespace chicken_inn {//----------- namespace: chicken_inn ----------------//

    inline std::uniform_int_distribution<int>     uDistribution_int(      3,   15 );
    inline std::uniform_real_distribution<double> uDistribution_double( -1.3, 1.3 );

    void fly( GameObj &goRef_, Chicken_PvtBinary *pvtBp_ ); // tmp

    void alti_manage( GameObj &goRef_, Chicken_PvtBinary *pvtBp_ );


}//-------------- namespace: chicken_inn end ----------------//




void Chicken::init(GameObj &goRef_, const DyParam &dyParams_ ){

    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<Chicken_PvtBinary>();

    //========== 标准化装配 ==========//
    assemble_regularGo( goRef_, dyParams_ );
    
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

    // 管理 鸡的 alti （飞行，掉落）
    chicken_inn::alti_manage( goRef_, pvtBp );

    if( pvtBp->isFlying ){
        chicken_inn::fly( goRef_, pvtBp );
    }

    // test .....
    Chicken::move_AnimAction_switch( goRef_ );

    goRef_.move.RenderUpdate();
            // 不应该直接调用它
            // 而是要在 条件合适时

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.goMeshSet.render_all_goMeshs_without_callback();
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
        goRef_.move.set_newCrawlDirAxes( DirAxes{pvtBp->moveVec} );
    }
    
    

}



void Chicken::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){


    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<Chicken_PvtBinary>();
    //=====================================//

    //auto dir = goRef_.actionDirection.get_newVal();
    //auto brokenLvl = goRef_.brokenLvl.get_newVal();

    //-- 获得所有 goMesh 的访问权 --
    GameObjMesh &goMeshRef = goRef_.goMeshSet.get_goMeshRef("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( type_ ){
        case ActionSwitchType::Idle:

            if( pvtBp->isFlying ){
                pvtBp->isFlying = false;
                pvtBp->isShakeFlying = false;
                pvtBp->flyCount = 0;
                pvtBp->shakeFlyCount = 0.0;
            }

            goMeshRef.set_animActionEName( AnimActionEName::Idle );
            goMeshRef.bind_animAction();
            goRef_.rebind_rootAnimActionPosPtr(); //- 临时性的方案 ...
            break;

        case ActionSwitchType::Move:

            // walk, run, fly
            // 使用一个 统一的函数 来管理            
            Chicken::moveState_manage(  goRef_, goMeshRef );

            break;

        default:
            break;
            //-- 并不报错，什么也不做...
    }

}


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


    //auto dir = goRef_.actionDirection.get_newVal();
    //auto brokenLvl = goRef_.brokenLvl.get_newVal();
    auto moveSpeedLvl = goRef_.move.moveSpeedLvl.get_newVal();

    auto *pvtBp = goRef_.get_pvtBinaryPtr<Chicken_PvtBinary>();


    goSpeciesId_t chickenId = GoSpecFromJson::str_2_goSpeciesId("chicken");
    GoSpecFromJson &goSpecFromJsonRef = GoSpecFromJson::getnc_goSpecFromJsonRef( chickenId );

    const auto &moveStateTable = *(goSpecFromJsonRef.moveStateTableUPtr);

    // 当 go moveSpeedLvl 越界时，不报错，而是强行改回 区间内
    // minLvl, maxLvl
    if( moveSpeedLvl < moveStateTable.minLvl ){
        moveSpeedLvl = moveStateTable.minLvl;
        goRef_.move.moveSpeedLvl.set_newVal( moveSpeedLvl ); // 可能会让 数据 dirty ...
    }else if( moveSpeedLvl > moveStateTable.maxLvl ){
        moveSpeedLvl = moveStateTable.maxLvl;
        goRef_.move.moveSpeedLvl.set_newVal( moveSpeedLvl ); // 可能会让 数据 dirty ...
    }

    // 最简单的映射，并不做加减速 .....
    //
    auto it = moveStateTable.table.find(moveSpeedLvl);
    tprAssert( it != moveStateTable.table.end() );
    AnimActionEName  actionEName = it->second.first;
    int timeStepOff = it->second.second;
        

    // 开启 fly 模式
    if( actionEName == AnimActionEName::Fly ){
        if( !pvtBp->isFlying ){
            // 只有第一次 检查到，才需要重设数据
            // 若已经处于飞行模式了，无需 reset
            pvtBp->isFlying = true;
            pvtBp->flyCount = 0;
        }
    }else{
        if( pvtBp->isFlying ){
            //pvtBp->isFlying = false;
            //pvtBp->isShakeFlying = false;
            //pvtBp->flyCount = 1000;
        }
    }
        
    goMeshRef_.set_animActionEName( actionEName );
    goMeshRef_.bind_animAction( timeStepOff );
    goRef_.rebind_rootAnimActionPosPtr(); //- 临时性的方案 ...

}




// regular-go in crawl-move
// 检查几个 go 数据，如果出现变化，将及时修正 aaction
//
//  将会被实现为一个，所有 活体go，通用的 自动处理 move-aaction 的 函数
//
void Chicken::move_AnimAction_switch( GameObj &goRef_ ){

    auto &crawlDirAxes = goRef_.move.crawlDirAxes;
    auto &moveSpeedLvl = goRef_.move.moveSpeedLvl;
    auto &actionDir    = goRef_.actionDirection; // 受 crawlDirAxes 影响的 被动值
    //...

    //---------------------//
    // 在 crawl-move 中，go.dir 是纯被动值，受 crawlDirAxes 影响
    // 如果发现此值 被人为改动，说明 本帧发生了 其他的优先级更高的 中断事件
    // 通常，此时不会调用 本函数 （说明流程没有做干净... ）
    tprAssert( !actionDir.get_isDirty() );

    // 不需要任何操作
    if( !crawlDirAxes.get_isDirty() &&
        !moveSpeedLvl.get_isDirty() &&
        !actionDir.get_isDirty() ){
        return;
    }

    //---------------------//
    NineDirection newDir = dirAxes_2_nineDirection( crawlDirAxes.get_newVal() );
    NineDirection oldDir = actionDir.get_oldVal();
    // 当本帧停止运动时，将保留原来的 go.dir 值 （此处的 Center值 是无意义的）    
    if( (newDir!=NineDirection::Center) && (newDir!=oldDir) ){
        actionDir.set_newVal( newDir );
        actionDir.sync(); // MUST 
                // 此时，oldDir 作为临时变量，数据没有被同步
    }  

    // switch aaction
    if( crawlDirAxes.get_oldVal().is_zero() ){
        // last frame is Idle
        if( !crawlDirAxes.get_newVal().is_zero() ){
            // this frame is Move
            goRef_.actionSwitch.call_func( ActionSwitchType::Move ); 
                                //-  move 存在很多种类：walk,run,fly
                                //   统一让 具象go类 去处理
        }
    }else{
        // last frame is Move
        if( crawlDirAxes.get_newVal().is_zero() ){
            // this frame is Idle
            goRef_.actionSwitch.call_func( ActionSwitchType::Idle );
        }else{
            // this frame is Move
            if( (newDir!=oldDir) ||          // 发生了有效的 方向变化
                (moveSpeedLvl.get_isDirty()) // 任何形式的 speedlvl 变化
            ){
                goRef_.actionSwitch.call_func( ActionSwitchType::Move ); 
            }
        }
    }

    //---------------------//
    // 目前对 moveSpeedLvl 的使用是很局限的：
    //   
    moveSpeedLvl.sync(); // maybe
    crawlDirAxes.sync(); // maybe
}


namespace chicken_inn {//----------- namespace: chicken_inn ----------------//


void fly( GameObj &goRef_, Chicken_PvtBinary *pvtBp_ ){

    // 飞行一定时长后，强制降速并变为 跑步
    if( pvtBp_->flyCount >= 60 ){
        pvtBp_->flyCount = 0;

        pvtBp_->isFlying = false;
        pvtBp_->isShakeFlying = false;
        goRef_.move.moveSpeedLvl.set_newVal( calc_lower_speedLvl(goRef_.move.moveSpeedLvl.get_newVal()) );
        return;
    }

    pvtBp_->flyCount++;
}



// 独立管理 鸡的 高度信息： 飞，自由落体
void alti_manage( GameObj &goRef_, Chicken_PvtBinary *pvtBp_ ){

    double highLimit = 40.0; 


    double goAlti = goRef_.get_pos_alti();

    if( pvtBp_->isFlying ){
        // 正在飞行状态

        if( pvtBp_->isShakeFlying ){
            // 进入 抖动飞行状态
            // cos 函数实现

            double step = 0.4;
            double amplitude = 7.0;

            goAlti = highLimit - amplitude + amplitude*cos(pvtBp_->shakeFlyCount * step);

            pvtBp_->shakeFlyCount++;

        }else{
            // 还在升空阶段，尚未进入 抖动飞行状态
            if( goAlti < highLimit ){
                goAlti += 1.5;
                if( goAlti >= highLimit ){
                    pvtBp_->isShakeFlying = true;
                    pvtBp_->shakeFlyCount = 0.0;
                }
            }
        }

        //---
        goRef_.set_pos_alti( goAlti );

    }else{
        // 非 飞行状态

        if( goAlti != 0.0 ){

            goAlti -= 3.5;
            if( goAlti < 0.0 ){
                goAlti = 0.0;
            }

            goRef_.set_pos_alti( goAlti );
        }
    }
}







}//-------------- namespace: chicken_inn end ----------------//
}//------------- namespace gameObjs: end ----------------

