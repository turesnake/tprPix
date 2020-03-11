/*
 * ================= create_go_by_hand.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.02.24
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * 
 *  临时版，使用类似 蓝图的流程，在主线程中，手动创建一个 go
 * 
 * -----
 */
#include "pch.h"
#include "create_goes.h"

//------------------- Engine --------------------//
#include "DiskGameObj.h"
#include "GoSpecFromJson.h"
#include "UIGoSpecFromJson.h"

#include "esrc_gameObj.h" 

//--------------- Script ------------------//
#include "Script/gameObjs/allGoes.h"
#include "Script/json/json_all.h"


namespace gameObjs{//------------- namespace gameObjs ----------------




// 根据 GoDataForCreate 数据
// 自动创建一个 DyParams_GoDataForCreate
// 然后 新建 go
goid_t create_a_Go( const GoDataForCreate *goDPtr_ ){

    //===== dyParam =====//
    DyParam dyParam {};
    auto fUPtr = std::make_unique<DyParams_GoDataForCreate>();
    fUPtr->goDataPtr = goDPtr_;
    dyParam.insert_ptr<DyParams_GoDataForCreate>( fUPtr.get() );


    //===== create a go =====//
    goid_t goid = esrc::insert_new_regularGo( goDPtr_->get_dpos(), goDPtr_->get_goUWeight() );
    // get go ref
    auto goOpt = esrc::get_goPtr( goid );
    tprAssert( goOpt.has_value() );
    GameObj &goRef = *goOpt.value();


    //-- set some static datas from JSON --
    tprAssert( GoSpecFromJson::find_from_initFuncs(goDPtr_->get_goSpeciesId()) );
    GoSpecFromJson::assemble_2_newGo( goDPtr_->get_goSpeciesId(), goRef );

    //-- check GameObjFamily --
        tprAssert( goRef.family != GameObjFamily::UI );

    GoSpecFromJson::call_initFunc( goDPtr_->get_goSpeciesId(), goRef, dyParam );

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





void rebind_a_diskGo( const DiskGameObj &diskGo_ ){

    auto goDataUPtr = GoDataForCreate::create_new_goDataForCreate(  
                                                    dpos_2_mpos(diskGo_.dpos),
                                                    diskGo_.dpos,
                                                    diskGo_.goSpeciesId,
                                                    diskGo_.goLabelId,
                                                    diskGo_.dir,
                                                    diskGo_.brokenLvl
                                                );
    //----- dyParam -----//
    DyParam dyParam {};
    auto fUPtr = std::make_unique<DyParams_GoDataForCreate>();
    fUPtr->goDataPtr = goDataUPtr.get();
    dyParam.insert_ptr<DyParams_GoDataForCreate>( fUPtr.get() );

    //----- rebind -----//
    esrc::insert_a_diskGo( diskGo_.goid, goDataUPtr->get_dpos(), diskGo_.goUWeight );
    // get go ref
    auto goOpt = esrc::get_goPtr( diskGo_.goid );
    tprAssert( goOpt.has_value() );
    GameObj &goRef = *goOpt.value();

    //-- set some static datas from JSON --
        tprAssert( GoSpecFromJson::find_from_initFuncs(goDataUPtr->get_goSpeciesId()) );
    GoSpecFromJson::assemble_2_newGo( goDataUPtr->get_goSpeciesId(), goRef ); //- tmp
                    //-- 临时措施
                    //   在未来，已经组装 从 数据库取出的数据，而不是从 GoSpecFromJson 中
                    //   至少有一部分吧
                    //   ...
    
    //-- check GameObjFamily --
        tprAssert( goRef.family != GameObjFamily::UI );

    GoSpecFromJson::call_initFunc( goDataUPtr->get_goSpeciesId(), goRef, dyParam );
            //-- 临时方案，最好使用 具象go类 rebind 系列函数 

    goRef.init_check();
    //------------------------------//
    signUp_newGO_to_chunk_and_mapEnt( goRef );
    esrc::insert_2_goids_inactive( diskGo_.goid );
            //- 放入 未激活队列会造成 5帧的 显示空缺
            //- 更为完善的做法是，当场检测应该放入 激活队列还是 未激活队列...
            //  未来被 GoMemState 系统取代
            //  ...
}





}//------------- namespace gameObjs: end ----------------

