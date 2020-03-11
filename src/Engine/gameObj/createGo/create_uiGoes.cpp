/*
 * ========================= create_uiGoes.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.04.10
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  初级版本
 * ----------
 */
#include "pch.h"
#include "create_goes.h"

//-------------------- Engine --------------------//
#include "GoSpecFromJson.h"
#include "UIGoSpecFromJson.h"

#include "esrc_gameObj.h" 


//--------------- Script ------------------//
#include "Script/gameObjs/allGoes.h"
#include "Script/json/json_all.h"



namespace uiGos{//------------- namespace uiGos ----------------


// 原始的方法，未来做整合
goid_t create_a_UIGo( goSpeciesId_t goSpeciesId_,
                    const glm::dvec2 &basePointProportion_, 
                    const glm::dvec2 &offDPos_,
                    const DyParam   &dyParams_,
                    size_t          goUWeight_ ){

    goid_t goid = esrc::insert_new_uiGo( basePointProportion_, offDPos_, goUWeight_ );
    // get go ref
    auto goOpt = esrc::get_goPtr( goid );
    tprAssert( goOpt.has_value() );
    GameObj &goRef = *goOpt.value();
    
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


// 原始的方法，未来做整合
goid_t create_a_UIGo( goSpeciesId_t goSpeciesId_,
                    const UIAnchor  &uiAnchor_,
                    const DyParam   &dyParams_,
                    size_t          goUWeight_ ){

    goid_t goid = esrc::insert_new_uiGo(uiAnchor_.get_basePointProportion(), 
                                        uiAnchor_.get_offDPos(),
                                        goUWeight_ );
    // get go ref
    auto goOpt = esrc::get_goPtr( goid );
    tprAssert( goOpt.has_value() );
    GameObj &goRef = *goOpt.value();
    
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



