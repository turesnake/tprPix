/*
 * ======================= BioSoup.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.18
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "Script/gameObjs/bioSoup/BioSoup.h"

//-------------------- Lib --------------------//
#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "animSubspeciesId.h"
#include "dyParams.h"
#include "assemble_go.h"
#include "GoDataForCreate.h"
#include "GoSpecFromJson.h"

//-------------------- Script --------------------//
#include "Script/gameObjs/bioSoup/BioSoupDataForCreate.h"
#include "Script/gameObjs/bioSoup/bioSoupInn.h"
#include "Script/gameObjs/bioSoup/BioSoupParticle.h"
#include "Script/gameObjs/bioSoup/BioSoupBase.h"


using namespace std::placeholders;


namespace gameObjs::bioSoup {//------------- namespace gameObjs::bioSoup ----------------
namespace bioSoup_inn {//------------------ namespace: bioSoup_inn ---------------------//


    class GoMesh_PvtBinary{
    public:
        GoMesh_PvtBinary()=default;
        bool isParticle {false};
        size_t animFrameCount {0}; // 动画计数器
    };


    bool isLocalInit {false};
    void init();


    double calc_playSpeed( MapAltitude mapAlti_ )noexcept;



}//--------------------- namespace: bioSoup_inn end ------------------------//




class BioSoup_PvtBinary{
public:
    BioSoup_PvtBinary():
        baseUPtr( std::make_unique<BioSoupBase>() ),
        particleUPtr( std::make_unique<BioSoupParticle>() )
        {}


    inline void init(   size_t uWeight_, 
                        State bioSoupState_, 
                        SignInMapEnts_Square_Type squType_,
                        MapAltitude mpAlti_
                    )noexcept{
        this->baseUPtr->init( uWeight_, bioSoupState_, squType_ );
        this->particleUPtr->init( uWeight_, bioSoupState_, squType_, mpAlti_ );
        this->bioSoupState = bioSoupState_;
        //---
        this->particleCreateCount = this->particleUPtr->get_next_createStep();

    }


    inline bool is_need_to_create_new_particle()noexcept{
        this->particleCreateCount--;
        if( this->particleCreateCount <= 0 ){
            this->particleCreateCount = this->particleUPtr->get_next_createStep();
            return true; // need to create new particle
        }else{
            return false;
        }   
    }


    inline std::string create_new_particleGoMeshName()noexcept{
        this->particleGoMeshIdx++;
        return tprGeneral::nameString_combine("particle_", this->particleGoMeshIdx, "");
    }


    // Exist but forbidden to call
    BioSoup_PvtBinary( const BioSoup_PvtBinary & ){ tprAssert(0); }
    BioSoup_PvtBinary &operator=( const BioSoup_PvtBinary & );


    std::unique_ptr<BioSoupBase>    baseUPtr {};
    std::unique_ptr<BioSoupParticle> particleUPtr {};
    
    State   bioSoupState {};
    double  playSpeed {};



private:

    size_t particleCreateCount {0}; // 生成 particle 递减计数器
    size_t particleGoMeshIdx {0}; // 累计生成了多少个 particle GoMesh, 生成 goMeshName 用
    
};




void BioSoup::init(GameObj &goRef_, const DyParam &dyParams_ ){

    if( !bioSoup_inn::isLocalInit ){
        bioSoup_inn::isLocalInit = true;
        bioSoup_inn::init();
    }

    //========== 标准化装配 ==========//
    const GoDataForCreate *goDPtr = assemble_regularGo( goRef_, dyParams_ );
    const DataForCreate *bioSoupDPtr = goDPtr->get_binary().get<DataForCreate>();


    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<BioSoup_PvtBinary>();
    pvtBp->init(    goRef_.get_goUWeight(), 
                    bioSoupDPtr->bioSoupState,
                    goDPtr->get_colliDataFromJsonPtr()->get_signInMapEnts_square_type(),
                    bioSoupDPtr->mapEntAlti
                );

    //--
    pvtBp->playSpeed = bioSoup_inn::calc_playSpeed( bioSoupDPtr->mapEntAlti );

  
    
    std::string rootGoMeshName = "root";
    auto &rootGoMeshRef = goRef_.goMeshSet.get_goMeshRef(rootGoMeshName);
    rootGoMeshRef.bind_reset_playSpeedScale( [pvtBp_l=pvtBp](){ return pvtBp_l->playSpeed; } );
    //---
    auto *goMeshPvtBp = rootGoMeshRef.init_pvtBinary<bioSoup_inn::GoMesh_PvtBinary>();
    goMeshPvtBp->isParticle = false; 

    // 每次 aaction动画播放完毕后，就切换一个动画
    goRef_.goMeshSet.bind_goMesh_callback_inLastFrame( rootGoMeshName,
        [pvtBp_l=pvtBp]( GameObjMesh &goMeshRef_ ){
            goMeshRef_.set_animSubspeciesId( pvtBp_l->baseUPtr->get_next_animSubspeciesId() );
            goMeshRef_.bind_animAction();
        }
    );
    

    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &BioSoup::OnRenderUpdate,  _1 );   
    
    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &BioSoup::OnActionSwitch,  _1, _2 ) );

    //================ go self vals =================//
}

void BioSoup::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){ tprAssert(0);}





void BioSoup::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<BioSoup_PvtBinary>();

    //------------------------//
    // 定期生成一个 particle gomesh
    
    if( pvtBp->is_need_to_create_new_particle() ){

        std::string goMeshName = pvtBp->create_new_particleGoMeshName();
        GameObjMesh &smokeGoMesh = goRef_.goMeshSet.creat_new_goMesh(
                                        goMeshName,
                                        pvtBp->particleUPtr->get_next_animSubspeciesId(),
                                        AnimActionEName::Rise,
                                        RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                        ShaderType::BioSoupParticle,  // pic shader
                                        pvtBp->particleUPtr->get_next_goMeshDposOff(), //- pposoff
                                        0.0,  //- zOff
                                        1151, // uweight tmp
                                        true //- isVisible
                                        );
        smokeGoMesh.set_alti( 0.0 );
        auto *particle_pvtBp = smokeGoMesh.init_pvtBinary<bioSoup_inn::GoMesh_PvtBinary>();
        particle_pvtBp->isParticle = true;

        // aaction 动画播放完毕后就 请求被销毁
        goRef_.goMeshSet.bind_goMesh_callback_inLastFrame( goMeshName,
            []( GameObjMesh &goMeshRef_ ){
                goMeshRef_.set_isNeedToBeErase(true);
            }
        );
    }
    
    goRef_.goMeshSet.render_all_goMeshs_with_callback();
}









namespace bioSoup_inn {//------------------ namespace: bioSoup_inn ---------------------//


// no need to called in main
void init(){
}



// mapAlti: -10 是动画播放最快的区域。越远离，越惰性
double calc_playSpeed( MapAltitude mapAlti_ )noexcept{
    double altiOff = static_cast<double>( std::abs( mapAlti_.get_val() - (-10) ) );
    altiOff /= 50.0;
    double playSpeed = 0.5 + altiOff;
    if( playSpeed > 3.0 ){
        playSpeed = 3.0;
    }
    return playSpeed;
}



}//--------------------- namespace: bioSoup_inn end ------------------------//
}//------------- namespace gameObjs::bioSoup: end ----------------

