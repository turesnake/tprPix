/*
 * ========================= create_goes.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "create_goes.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "GoSpecFromJson.h"
#include "UIGoSpecFromJson.h"

#include "esrc_gameObj.h" 


//--------------- Script ------------------//
#include "Script/gameObjs/allGoes.h"

#include "Script/json/json_all.h"

#include "tprDebug.h"


namespace gameObjs{//------------- namespace gameObjs ----------------


/* ===========================================================
 *                  create_a_Go       [tmp]
 * -----------------------------------------------------------
 * -- 仅适用于 部分类型的 go
 * -- 对于很多类似 ui的，不依赖map，且更为轻量的go，应为其专设 create 函数 
 */
goid_t create_a_Go( goSpeciesId_t goSpeciesId_,
                    const glm::dvec2 &dpos_,
                    const DyParam &dyParams_ ){

    goid_t goid = esrc::insert_new_regularGo( dpos_ );
    GameObj &goRef = esrc::get_goRef( goid );

    //-- set some static datas from JSON --
        tprAssert( GoSpecFromJson::find_from_initFuncs(goSpeciesId_) );
    GoSpecFromJson::assemble_2_newGo( goSpeciesId_, goRef );

    //-- check GameObjFamily --
        tprAssert( goRef.family != GameObjFamily::UI );

    GoSpecFromJson::call_initFunc( goSpeciesId_, goRef, dyParams_ );

    goRef.init_check();
    //------------------------------//
    signUp_newGO_to_chunk_and_mapEnt( goRef );
    esrc::insert_2_goids_inactive( goid );
            //- 放入 未激活队列会造成 5帧的 显示空缺
            //- 更为完善的做法是，当场检测应该放入 激活队列还是 未激活队列...
            //  未来被 GoMemState 系统取代
            //  ...
    
    return  goid;
}


/* ===========================================================
 *                  rebind_a_disk_Go       [tmp]
 * -----------------------------------------------------------
 * 从 db读取一个 go 的数据，并用此数据，重建一个 mem态 go实例
 */
void rebind_a_disk_Go(  goid_t          diskGoId_,
                        goSpeciesId_t   diskGoSpeciesId_,
                        const glm::dvec2 &dpos_,
                        const DyParam &dyParams_  ){

    esrc::insert_a_diskGo( diskGoId_, dpos_ );
    GameObj &goRef = esrc::get_goRef( diskGoId_ );

    //-- set some static datas from JSON --
        tprAssert( GoSpecFromJson::find_from_initFuncs(diskGoSpeciesId_) );
    GoSpecFromJson::assemble_2_newGo( diskGoSpeciesId_, goRef ); //- tmp
                    //-- 临时措施
                    //   在未来，已经组装 从 数据库取出的数据，而不是从 GoSpecFromJson 中
                    //   至少有一部分吧
                    //   ...
    
    //-- check GameObjFamily --
        tprAssert( goRef.family != GameObjFamily::UI );

    GoSpecFromJson::call_initFunc( diskGoSpeciesId_, goRef, dyParams_ );
            //-- 临时方案，最好使用 具象go类 rebind 系列函数 

    goRef.init_check();
    //------------------------------//
    signUp_newGO_to_chunk_and_mapEnt( goRef );
    esrc::insert_2_goids_inactive( diskGoId_ );
            //- 放入 未激活队列会造成 5帧的 显示空缺
            //- 更为完善的做法是，当场检测应该放入 激活队列还是 未激活队列...
            //  未来被 GoMemState 系统取代
            //  ...
}


}//------------- namespace gameObjs: end ----------------



namespace uiGos{//------------- namespace uiGos ----------------



/* ===========================================================
 *                  create_a_UIGo
 * -----------------------------------------------------------
 */
goid_t create_a_UIGo( goSpeciesId_t goSpeciesId_,
                    const glm::dvec2 &basePointProportion_, 
                    const glm::dvec2 &offDPos_,
                    const DyParam &dyParams_ ){

    goid_t goid = esrc::insert_new_uiGo( basePointProportion_, offDPos_ );
    GameObj &goRef = esrc::get_goRef( goid );
    
    //-- set some static datas from JSON --
        tprAssert( UIGoSpecFromJson::find_from_initFuncs(goSpeciesId_) );
    UIGoSpecFromJson::assemble_2_newUIGo( goSpeciesId_, goRef );

    //-- check GameObjFamily --
    tprAssert( goRef.family == GameObjFamily::UI );

    UIGoSpecFromJson::call_initFunc( goSpeciesId_, goRef, dyParams_ );

    goRef.init_check();
    //------------------------------//
    //  uiGo 不用登记到 map 中，目前来看，是被一个 生命周期稳定的 scene 手动管理
    //  ui 也不存在什么 active 状态
    return  goid;
}


goid_t create_a_UIGo( goSpeciesId_t goSpeciesId_,
                    const UIAnchor &uiAnchor_,
                    const DyParam &dyParams_ ){

    goid_t goid = esrc::insert_new_uiGo(uiAnchor_.get_basePointProportion(), 
                                        uiAnchor_.get_offDPos() );
    GameObj &goRef = esrc::get_goRef( goid );
    
    //-- set some static datas from JSON --
        tprAssert( UIGoSpecFromJson::find_from_initFuncs(goSpeciesId_) );
    UIGoSpecFromJson::assemble_2_newUIGo( goSpeciesId_, goRef );

    //-- check GameObjFamily --
    tprAssert( goRef.family == GameObjFamily::UI );

    UIGoSpecFromJson::call_initFunc( goSpeciesId_, goRef, dyParams_ );
    
    goRef.init_check();
    //------------------------------//
    //  uiGo 不用登记到 map 中，目前来看，是被一个 生命周期稳定的 scene 手动管理
    //  ui 也不存在什么 active 状态
    return  goid;
}


}//------------- namespace uiGos: end ----------------



