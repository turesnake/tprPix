/*
 * ======================= BioSoup.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.18
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/bioSoup/BioSoup.h"

//-------------------- CPP --------------------//
#include <cmath>
#include <functional>
#include <string>
#include <vector>

//-------------------- Lib --------------------//
#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "Density.h"
#include "animSubspeciesId.h"
#include "RenderPool.h"
#include "create_go_oth.h"
#include "dyParams.h"
#include "assemble_go.h"
#include "GoDataForCreate.h"
#include "GoSpecFromJson.h"

#include "esrc_shader.h" 
#include "esrc_chunk.h"
#include "esrc_ecoObj.h"
#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//
#include "Script/gameObjs/bioSoup/BioSoupDataForCreate.h"
#include "Script/gameObjs/bioSoup/bioSoupInn.h"



using namespace std::placeholders;

#include "tprDebug.h" 


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






struct FloorGo_PvtBinary{

    State   bioSoupState {};
    double  playSpeed {};
    //===== padding =====//
    //...
};


void BioSoup::init(GameObj &goRef_, const DyParam &dyParams_ ){

    if( !bioSoup_inn::isLocalInit ){
        bioSoup_inn::isLocalInit = true;
        bioSoup_inn::init();
    }



    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<FloorGo_PvtBinary>();

    //========== 标准化装配 ==========//
    const GoDataForCreate *goDPtr = assemble_regularGo( goRef_, dyParams_ );

    const DataForCreate *bioSoupDPtr = goDPtr->get_binary().get<DataForCreate>();
    pvtBp->bioSoupState = bioSoupDPtr->bioSoupState;

    //--
    pvtBp->playSpeed = bioSoup_inn::calc_playSpeed( bioSoupDPtr->mapEntAlti );

        // cout << "playSpeed: " << pvtBp->playSpeed << endl;
    
    for( auto &[goMeshName, goMeshUPtr] : goRef_.get_goMeshs() ){
        goMeshUPtr->bind_reset_playSpeedScale( [=](){ return pvtBp->playSpeed; } );
        //---
        auto *goMeshPvtBp = goMeshUPtr->init_pvtBinary<bioSoup_inn::GoMesh_PvtBinary>();
        goMeshPvtBp->isParticle = false; 
    }



    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &BioSoup::OnRenderUpdate,  _1 );   
    
    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &BioSoup::OnActionSwitch,  _1, _2 ) );

    //================ go self vals =================//
}

void BioSoup::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){ tprAssert(0);}





void BioSoup::OnRenderUpdate( GameObj &goRef_ ){














    goRef_.render_all_goMesh();
}









namespace bioSoup_inn {//------------------ namespace: bioSoup_inn ---------------------//


// no need to called in main
void init(){

    init_for_particle();




}



// mapAlti: -10 是动画播放最快的区域。越远离，越惰性
double calc_playSpeed( MapAltitude mapAlti_ )noexcept{
    double altiOff = static_cast<double>( std::abs( mapAlti_.val - (-10) ) );
    altiOff /= 50.0;
    double playSpeed = 0.5 + altiOff;
    if( playSpeed > 3.0 ){
        playSpeed = 3.0;
    }
    return playSpeed;
}


/*
void create_particle_goMesh( GameObj goRef_, animSubspeciesId_t animSubId_ ){



    GameObjMesh &smokeGoMesh = goRef_.creat_new_goMesh(goMeshName, //- gmesh-name
                                            animSubId_
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


}
*/




}//--------------------- namespace: bioSoup_inn end ------------------------//
}//------------- namespace gameObjs::bioSoup: end ----------------

