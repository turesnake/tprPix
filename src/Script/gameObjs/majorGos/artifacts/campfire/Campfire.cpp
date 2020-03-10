/*
 * ========================= Campfire.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.11.16
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Script/gameObjs/majorGos/artifacts/campfire/Campfire.h"

//-------------------- Lib --------------------//
#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "animSubspeciesId.h"
#include "dyParams.h"
#include "assemble_go.h"

#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//


using namespace std::placeholders;


namespace gameObjs {//------------- namespace gameObjs ----------------






class Campfire_PvtBinary{
public:

    inline bool is_need_to_create_new_smoke()noexcept{
        this->smoke_create_count--;
        if( this->smoke_create_count <= 0 ){
            this->smoke_create_count = 90;
            return true; // need to create new smoke
        }else{
            return false;
        }
    }

    inline std::string create_next_smokeGoMeshName()noexcept{
        this->smokeGoMeshIdx++;
        return tprGeneral::nameString_combine("smoke_", this->smokeGoMeshIdx, "");
    }


    //--- smoke gomesh ---
    animSubspeciesId_t smokeSubId {};

private:
    int smoke_create_count {0}; // 生成 smoke gomesh 的递减计数器
    size_t smokeGoMeshIdx {0}; // 累计生成了多少个 smokeGoMesh, 生成 goMeshName 用
};


namespace campfire_inn {//----------- namespace: campfire_inn ----------------//

    // 目前主要用来存储 smoke gomesh 数据
    class GoMesh_PvtBinary{
    public:
        GoMesh_PvtBinary()=default;
        bool isSmoke {false};
        size_t animFrameCount {0}; // 动画计数器
    };

    void update_for_smokeGoMesh( GameObjMesh &smokeGoMesh_, GoMesh_PvtBinary *goMeshBp_ );

}//-------------- namespace: campfire_inn end ----------------//



void Campfire::init(GameObj &goRef_,const DyParam &dyParams_ ){


    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<Campfire_PvtBinary>();

    // 以后使用，先准备起来
    pvtBp->smokeSubId = esrc::apply_a_random_animSubspeciesId( "campfire_smoke", "", 7 );

    //========== 标准化装配 ==========//
    assemble_regularGo( goRef_, dyParams_ );

    //========== goMeshs =========//
    GameObjMesh &rootGoMesh = goRef_.goMeshSet.get_goMeshRef("root");
    auto *root_pvtBp = rootGoMesh.init_pvtBinary<campfire_inn::GoMesh_PvtBinary>();
    root_pvtBp->isSmoke = false; 

    GameObjMesh &fireGoMesh = goRef_.goMeshSet.get_goMeshRef("fire");
    auto *fire_pvtBp = fireGoMesh.init_pvtBinary<campfire_inn::GoMesh_PvtBinary>();
    fire_pvtBp->isSmoke = false; 
 
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &Campfire::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &Campfire::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &Campfire::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );
    goRef_.actionSwitch.signUp( ActionSwitchType::Burn );

    //================ go self vals =================//   

}

/* -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void Campfire::bind( GameObj &goRef_ ){
}


/* -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void Campfire::rebind( GameObj &goRef_ ){
}


void Campfire::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<Campfire_PvtBinary>();

    //------------------------//
    // 定期生成一个 smoke gomesh 
    if( pvtBp->is_need_to_create_new_smoke() ){

        std::string goMeshName = pvtBp->create_next_smokeGoMeshName();
        GameObjMesh &smokeGoMesh = goRef_.goMeshSet.creat_new_goMesh(
                                        goMeshName,
                                        pvtBp->smokeSubId,
                                        AnimActionEName::Burn,
                                        RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                        ShaderType::UnifiedColor,  // pic shader
                                        glm::dvec2{}, //- pposoff
                                        0.2,  //- zOff: 在 fire 上方
                                        1151, // uweight tmp
                                        true //- isVisible
                                        );
        smokeGoMesh.set_alti( 70.0 );
        auto *smoke_pvtBp = smokeGoMesh.init_pvtBinary<campfire_inn::GoMesh_PvtBinary>();
        smoke_pvtBp->isSmoke = true;

        goRef_.goMeshSet.bind_goMesh_callback_everyFrame( goMeshName,
            []( GameObjMesh &goMeshRef_ ){
                auto *goMeshPvtBp = goMeshRef_.get_pvtBinaryPtr<campfire_inn::GoMesh_PvtBinary>();
                campfire_inn::update_for_smokeGoMesh( goMeshRef_, goMeshPvtBp );
            }
        );

        // aaction 动画播放完毕后就 请求被销毁
        goRef_.goMeshSet.bind_goMesh_callback_inLastFrame( goMeshName,
            []( GameObjMesh &goMeshRef_ ){
                goMeshRef_.set_isNeedToBeErase(true);
            }
        );
    }

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.goMeshSet.render_all_goMeshs_with_callback();
}

void Campfire::OnLogicUpdate( GameObj &goRef_ ){
}


// 暂未实现
void Campfire::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){
    tprAssert(0);
}



namespace campfire_inn {//----------- namespace: campfire_inn ----------------//



void update_for_smokeGoMesh( GameObjMesh &smokeGoMesh_, GoMesh_PvtBinary *goMeshBp_ ){

    goMeshBp_->animFrameCount++;
    double animDCount = static_cast<double>(goMeshBp_->animFrameCount);

    smokeGoMesh_.accum_alti( 0.6 ); // 不停增高
    smokeGoMesh_.accum_zOff( 0.001 ); // zOff 递增，确保能覆盖 低处的 smoke
    smokeGoMesh_.accum_pposOff( glm::dvec2{ 0.005 * animDCount, 0.0 } ); // 随着不停升高，飘向右侧，表现风
}



}//-------------- namespace: campfire_inn end ----------------//
}//------------- namespace gameObjs: end ----------------

